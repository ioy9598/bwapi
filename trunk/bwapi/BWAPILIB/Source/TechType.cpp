#include <string>
#include <map>
#include <set>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingTechType = true;
  class TechTypeInternal
  {
    public:
      TechTypeInternal() {valid = false;}
      void set(const char* name, int mineralPrice, int gasPrice, int researchTime, int energyUsed, UnitType whatResearches, Race race, WeaponType weapon, bool targetsUnit, bool targetsPosition, Order orderType, UnitType whatUses1, UnitType whatUses2=UnitTypes::None, UnitType whatUses3=UnitTypes::None, UnitType whatUses4=UnitTypes::None, UnitType whatUses5=UnitTypes::None, UnitType whatUses6=UnitTypes::None)
      {
        this->name           = name;
        this->mineralPrice   = mineralPrice;
        this->gasPrice       = gasPrice;
        this->researchTime   = researchTime;
        this->energyUsed     = energyUsed;
        this->whatResearches = whatResearches;
        this->race           = race;
        this->weapon         = weapon;

        this->targetsUnit     = targetsUnit;
        this->targetsPosition = targetsPosition;
        this->orderType       = orderType;

        if (whatUses1 != UnitTypes::None)
          this->whatUses.insert(whatUses1);

        if (whatUses2 != UnitTypes::None)
          this->whatUses.insert(whatUses2);

        if (whatUses3 != UnitTypes::None)
          this->whatUses.insert(whatUses3);

        if (whatUses4 != UnitTypes::None)
          this->whatUses.insert(whatUses4);

        if (whatUses5 != UnitTypes::None)
          this->whatUses.insert(whatUses5);

        if (whatUses6 != UnitTypes::None)
          this->whatUses.insert(whatUses6);

        this->valid = true;
      }
      std::string name;
      int mineralPrice;
      int gasPrice;
      int researchTime;
      int energyUsed;
      UnitType whatResearches;
      Race race;
      WeaponType weapon;
      bool targetsUnit;
      bool targetsPosition;
      std::set<UnitType> whatUses;
      Order orderType;

      bool valid;
  };
  TechTypeInternal techTypeData[47];
  std::map<std::string, TechType> techTypeMap;
  std::set< TechType > techTypeSet;
  namespace TechTypes
  {
    const TechType Stim_Packs(0);
    const TechType Lockdown(1);
    const TechType EMP_Shockwave(2);
    const TechType Spider_Mines(3);
    const TechType Scanner_Sweep(4);
    const TechType Tank_Siege_Mode(5);
    const TechType Defensive_Matrix(6);
    const TechType Irradiate(7);
    const TechType Yamato_Gun(8);
    const TechType Cloaking_Field(9);
    const TechType Personnel_Cloaking(10);
    const TechType Burrowing(11);
    const TechType Infestation(12);
    const TechType Spawn_Broodlings(13);
    const TechType Dark_Swarm(14);
    const TechType Plague(15);
    const TechType Consume(16);
    const TechType Ensnare(17);
    const TechType Parasite(18);
    const TechType Psionic_Storm(19);
    const TechType Hallucination(20);
    const TechType Recall(21);
    const TechType Stasis_Field(22);
    const TechType Archon_Warp(23);
    const TechType Restoration(24);
    const TechType Disruption_Web(25);
    const TechType Mind_Control(27);
    const TechType Dark_Archon_Meld(28);
    const TechType Feedback(29);
    const TechType Optical_Flare(30);
    const TechType Maelstrom(31);
    const TechType Lurker_Aspect(32);
    const TechType Healing(34);
    const TechType None(44);
    const TechType Unknown(45);
    const TechType Nuclear_Strike(46);

    void init()
    {
      techTypeData[Stim_Packs].set("Stim Packs"                ,100,100,1200,0  ,UnitTypes::Terran_Academy          ,Races::Terran ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Terran_Marine, UnitTypes::Terran_Firebat, UnitTypes::Hero_Jim_Raynor_Marine, UnitTypes::Hero_Gui_Montag);
      techTypeData[Lockdown].set("Lockdown"                    ,200,200,1500,100,UnitTypes::Terran_Covert_Ops       ,Races::Terran ,WeaponTypes::Lockdown        , true,  false, Orders::CastLockdown, UnitTypes::Terran_Ghost, UnitTypes::Hero_Alexei_Stukov, UnitTypes::Hero_Infested_Duran, UnitTypes::Hero_Samir_Duran, UnitTypes::Hero_Sarah_Kerrigan);
      techTypeData[EMP_Shockwave].set("EMP Shockwave"          ,200,200,1800,100,UnitTypes::Terran_Science_Facility ,Races::Terran ,WeaponTypes::EMP_Shockwave   , true,  true,  Orders::CastEMPShockwave, UnitTypes::Terran_Science_Vessel, UnitTypes::Hero_Magellan);
      techTypeData[Spider_Mines].set("Spider Mines"            ,100,100,1200,0  ,UnitTypes::Terran_Machine_Shop     ,Races::Terran ,WeaponTypes::Spider_Mines    , false, true,  Orders::PlaceMine, UnitTypes::Terran_Vulture, UnitTypes::Hero_Jim_Raynor_Vulture);
      techTypeData[Scanner_Sweep].set("Scanner Sweep"          ,0  ,0  ,0   ,50 ,UnitTypes::None                    ,Races::Terran ,WeaponTypes::None            , true,  true,  Orders::CastScannerSweep, UnitTypes::Terran_Comsat_Station);
      techTypeData[Tank_Siege_Mode].set("Tank Siege Mode"      ,150,150,1200,0  ,UnitTypes::Terran_Machine_Shop     ,Races::Terran ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Terran_Siege_Tank_Tank_Mode, UnitTypes::Terran_Siege_Tank_Siege_Mode, UnitTypes::Hero_Edmund_Duke_Tank_Mode, UnitTypes::Hero_Edmund_Duke_Siege_Mode);
      techTypeData[Defensive_Matrix].set("Defensive Matrix"    ,0  ,0  ,0   ,100,UnitTypes::None                    ,Races::Terran ,WeaponTypes::None            , true,  false, Orders::CastDefensiveMatrix, UnitTypes::Terran_Science_Vessel, UnitTypes::Hero_Magellan);
      techTypeData[Irradiate].set("Irradiate"                  ,200,200,1200,75 ,UnitTypes::Terran_Science_Facility ,Races::Terran ,WeaponTypes::Irradiate       , true,  false, Orders::CastIrradiate, UnitTypes::Terran_Science_Vessel, UnitTypes::Hero_Magellan);
      techTypeData[Yamato_Gun].set("Yamato Gun"                ,100,100,1800,150,UnitTypes::Terran_Physics_Lab      ,Races::Terran ,WeaponTypes::Yamato_Gun      , true,  false, Orders::FireYamatoGun, UnitTypes::Terran_Battlecruiser, UnitTypes::Hero_Gerard_DuGalle, UnitTypes::Hero_Hyperion, UnitTypes::Hero_Norad_II);
      techTypeData[Cloaking_Field].set("Cloaking Field"        ,150,150,1500,25 ,UnitTypes::Terran_Control_Tower    ,Races::Terran ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Terran_Wraith, UnitTypes::Hero_Tom_Kazansky);
      techTypeData[Personnel_Cloaking].set("Personnel Cloaking",100,100,1200,25 ,UnitTypes::Terran_Covert_Ops       ,Races::Terran ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Terran_Ghost, UnitTypes::Hero_Alexei_Stukov, UnitTypes::Hero_Infested_Duran, UnitTypes::Hero_Samir_Duran, UnitTypes::Hero_Sarah_Kerrigan, UnitTypes::Hero_Infested_Kerrigan);
      techTypeData[Burrowing].set("Burrowing"                  ,100,100,1200,0  ,UnitTypes::Zerg_Hatchery           ,Races::Zerg   ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Zerg_Drone, UnitTypes::Zerg_Zergling, UnitTypes::Zerg_Hydralisk, UnitTypes::Zerg_Defiler, UnitTypes::Zerg_Infested_Terran, UnitTypes::Zerg_Lurker);
      techTypeData[Infestation].set("Infestation"              ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::Zerg   ,WeaponTypes::None            , true,  false, Orders::CastInfestation, UnitTypes::Zerg_Queen, UnitTypes::Hero_Matriarch);
      techTypeData[Spawn_Broodlings].set("Spawn Broodlings"    ,100,100,1200,150,UnitTypes::Zerg_Queens_Nest        ,Races::Zerg   ,WeaponTypes::Spawn_Broodlings, true,  false, Orders::CastSpawnBroodlings, UnitTypes::Zerg_Queen, UnitTypes::Hero_Matriarch);
      techTypeData[Dark_Swarm].set("Dark Swarm"                ,0  ,0  ,0   ,100,UnitTypes::None                    ,Races::Zerg   ,WeaponTypes::Dark_Swarm      , true,  true,  Orders::CastDarkSwarm, UnitTypes::Zerg_Defiler, UnitTypes::Hero_Unclean_One);
      techTypeData[Plague].set("Plague"                        ,200,200,1500,150,UnitTypes::Zerg_Defiler_Mound      ,Races::Zerg   ,WeaponTypes::Plague          , true,  true,  Orders::CastPlague, UnitTypes::Zerg_Defiler, UnitTypes::Hero_Unclean_One);
      techTypeData[Consume].set("Consume"                      ,100,100,1500,0  ,UnitTypes::Zerg_Defiler_Mound      ,Races::Zerg   ,WeaponTypes::Consume         , true,  false, Orders::CastConsume, UnitTypes::Zerg_Defiler, UnitTypes::Hero_Unclean_One, UnitTypes::Hero_Infested_Kerrigan);
      techTypeData[Ensnare].set("Ensnare"                      ,100,100,1200,75 ,UnitTypes::Zerg_Queens_Nest        ,Races::Zerg   ,WeaponTypes::Ensnare         , true,  true,  Orders::CastEnsnare, UnitTypes::Zerg_Queen, UnitTypes::Hero_Matriarch, UnitTypes::Hero_Infested_Kerrigan);
      techTypeData[Parasite].set("Parasite"                    ,0  ,0  ,0   ,75 ,UnitTypes::None                    ,Races::Zerg   ,WeaponTypes::Parasite        , true,  false, Orders::CastParasite, UnitTypes::Zerg_Queen, UnitTypes::Hero_Matriarch);
      techTypeData[Psionic_Storm].set("Psionic Storm"          ,200,200,1800,75 ,UnitTypes::Protoss_Templar_Archives,Races::Protoss,WeaponTypes::Psionic_Storm   , true,  true,  Orders::CastPsionicStorm, UnitTypes::Protoss_High_Templar, UnitTypes::Hero_Tassadar, UnitTypes::Hero_Infested_Kerrigan);
      techTypeData[Hallucination].set("Hallucination"          ,150,150,1200,100,UnitTypes::Protoss_Templar_Archives,Races::Protoss,WeaponTypes::None            , true,  false, Orders::CastHallucination, UnitTypes::Protoss_High_Templar, UnitTypes::Hero_Tassadar);
      techTypeData[Recall].set("Recall"                        ,150,150,1800,150,UnitTypes::Protoss_Arbiter_Tribunal,Races::Protoss,WeaponTypes::None            , true,  true,  Orders::CastRecall, UnitTypes::Protoss_Arbiter, UnitTypes::Hero_Danimoth);
      techTypeData[Stasis_Field].set("Stasis Field"            ,150,150,1500,100,UnitTypes::Protoss_Arbiter_Tribunal,Races::Protoss,WeaponTypes::Stasis_Field    , true,  true,  Orders::CastStasisField, UnitTypes::Protoss_Arbiter, UnitTypes::Hero_Danimoth);
      techTypeData[Archon_Warp].set("Archon Warp"              ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::Protoss,WeaponTypes::None            , true,  false, Orders::None, UnitTypes::Protoss_High_Templar);
      techTypeData[Restoration].set("Restoration"              ,100,100,1200,50 ,UnitTypes::Terran_Academy          ,Races::Terran ,WeaponTypes::Restoration     , true,  false, Orders::CastRestoration, UnitTypes::Terran_Medic);
      techTypeData[Disruption_Web].set("Disruption Web"        ,200,200,1200,125,UnitTypes::Protoss_Fleet_Beacon    ,Races::Protoss,WeaponTypes::Disruption_Web  , true,  true,  Orders::CastDisruptionWeb, UnitTypes::Protoss_Corsair, UnitTypes::Hero_Raszagal);
      techTypeData[Mind_Control].set("Mind Control"            ,200,200,1800,150,UnitTypes::Protoss_Templar_Archives,Races::Protoss,WeaponTypes::Mind_Control    , true,  false, Orders::CastMindControl, UnitTypes::Protoss_Dark_Archon);
      techTypeData[Dark_Archon_Meld].set("Dark Archon Meld"    ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::Protoss,WeaponTypes::None            , true,  false, Orders::None, UnitTypes::Protoss_Dark_Templar);
      techTypeData[Feedback].set("Feedback"                    ,0  ,0  ,0   ,50 ,UnitTypes::None                    ,Races::Protoss,WeaponTypes::Feedback        , true,  false, Orders::CastFeedback, UnitTypes::Protoss_Dark_Archon);
      techTypeData[Optical_Flare].set("Optical Flare"          ,100,100,1800,75 ,UnitTypes::Terran_Academy          ,Races::Terran ,WeaponTypes::Optical_Flare   , true,  false, Orders::CastOpticalFlare, UnitTypes::Terran_Medic);
      techTypeData[Maelstrom].set("Maelstrom"                  ,100,100,1500,100,UnitTypes::Protoss_Templar_Archives,Races::Protoss,WeaponTypes::Maelstrom       , true,  true,  Orders::CastMaelstrom, UnitTypes::Protoss_Dark_Archon);
      techTypeData[Lurker_Aspect].set("Lurker Aspect"          ,200,200,1800,0  ,UnitTypes::Zerg_Hydralisk_Den      ,Races::Zerg   ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::Zerg_Lurker);
      techTypeData[Healing].set("Healing"                      ,0  ,0  ,0   ,1  ,UnitTypes::None                    ,Races::Terran ,WeaponTypes::None            , true,  true,  Orders::MedicHeal1, UnitTypes::Terran_Medic);
      techTypeData[None].set("None"                            ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::None   ,WeaponTypes::None            , false, false, Orders::None, UnitTypes::None);
      techTypeData[Unknown].set("Unknown"                      ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::Unknown,WeaponTypes::None            , false, false, Orders::None, UnitTypes::None);
      techTypeData[Nuclear_Strike].set("Nuclear Strike"        ,0  ,0  ,0   ,0  ,UnitTypes::None                    ,Races::Terran ,WeaponTypes::Nuclear_Strike  , true,  true,  Orders::NukePaint, UnitTypes::Terran_Ghost);

      techTypeSet.insert(Stim_Packs);
      techTypeSet.insert(Lockdown);
      techTypeSet.insert(EMP_Shockwave);
      techTypeSet.insert(Spider_Mines);
      techTypeSet.insert(Scanner_Sweep);
      techTypeSet.insert(Tank_Siege_Mode);
      techTypeSet.insert(Defensive_Matrix);
      techTypeSet.insert(Irradiate);
      techTypeSet.insert(Yamato_Gun);
      techTypeSet.insert(Cloaking_Field);
      techTypeSet.insert(Personnel_Cloaking);
      techTypeSet.insert(Burrowing);
      techTypeSet.insert(Infestation);
      techTypeSet.insert(Spawn_Broodlings);
      techTypeSet.insert(Dark_Swarm);
      techTypeSet.insert(Plague);
      techTypeSet.insert(Consume);
      techTypeSet.insert(Ensnare);
      techTypeSet.insert(Parasite);
      techTypeSet.insert(Psionic_Storm);
      techTypeSet.insert(Hallucination);
      techTypeSet.insert(Recall);
      techTypeSet.insert(Stasis_Field);
      techTypeSet.insert(Archon_Warp);
      techTypeSet.insert(Restoration);
      techTypeSet.insert(Disruption_Web);
      techTypeSet.insert(Mind_Control);
      techTypeSet.insert(Dark_Archon_Meld);
      techTypeSet.insert(Feedback);
      techTypeSet.insert(Optical_Flare);
      techTypeSet.insert(Maelstrom);
      techTypeSet.insert(Lurker_Aspect);
      techTypeSet.insert(Healing);
      techTypeSet.insert(None);
      techTypeSet.insert(Unknown);
      techTypeSet.insert(Nuclear_Strike);

      foreach(TechType i, techTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        techTypeMap.insert(std::make_pair(name, i));
      }
      initializingTechType = false;
    }
  }
  TechType::TechType() : Type(TechTypes::None)
  {
  }
  int getValidTechTypeID(int id)
  {
    if ( !initializingTechType && (id < 0 || id >= 47 || techTypeData[id].name.length() == 0) )
      return TechTypes::Unknown;
    return id;
  }
  TechType::TechType(int id) : Type( getValidTechTypeID(id) )
  {
  }
  const std::string &TechType::getName() const
  {
    return techTypeData[this->getID()].name;
  }
  const char *TechType::c_str() const
  {
    return techTypeData[this->getID()].name.c_str();
  }
  Race TechType::getRace() const
  {
    return techTypeData[this->getID()].race;
  }
  int TechType::mineralPrice() const
  {
    return techTypeData[this->getID()].mineralPrice;
  }
  int TechType::gasPrice() const
  {
    return techTypeData[this->getID()].gasPrice;
  }
  int TechType::researchTime() const
  {
    return techTypeData[this->getID()].researchTime;
  }
  int TechType::energyUsed() const
  {
    return techTypeData[this->getID()].energyUsed;
  }
  UnitType TechType::whatResearches() const
  {
    return techTypeData[this->getID()].whatResearches;
  }
  WeaponType TechType::getWeapon() const
  {
    return techTypeData[this->getID()].weapon;
  }
  bool TechType::targetsUnit() const
  {
    return techTypeData[this->getID()].targetsUnit;
  }
  bool TechType::targetsPosition() const
  {
    return techTypeData[this->getID()].targetsPosition;
  }
  const std::set<UnitType>& TechType::whatUses() const
  {
    return techTypeData[this->getID()].whatUses;
  }
  Order TechType::getOrder() const
  {
    return techTypeData[this->getID()].orderType;
  }
  TechType TechTypes::getTechType(std::string name)
  {
    fixName(&name);
    std::map<std::string, TechType>::iterator i = techTypeMap.find(name);
    if (i == techTypeMap.end()) 
      return TechTypes::Unknown;
    return (*i).second;
  }
  const std::set<TechType>& TechTypes::allTechTypes()
  {
    return techTypeSet;
  }
}

