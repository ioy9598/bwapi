#pragma once
#include <BWAPI.h>
#include <Util/Foreach.h>
namespace BWAPI
{
  namespace Templates
  {
    //--------------------------------------------- HAS POWER ------------------------------------------------
    const bool bPsiFieldMask[10][16] = {
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }
    };
    template < class UnitImpl >
    bool hasPower(int x, int y, UnitType unitType, const std::set<UnitImpl*> &pylons)
    {
      if ( unitType >= 0 && unitType < UnitTypes::None && (!unitType.requiresPsi() || !unitType.isBuilding()) )
        return true;

      /* Loop through all pylons for the current player */
      foreach (UnitImpl* i, pylons)
      {
        if ( !i->exists() || !i->isCompleted() )
          continue;

        Position p = i->getPosition();
        if ( abs(p.x() - x) >= 256 )
          continue;

        if ( abs(p.y() - y) >= 160 )
          continue;

        if ( bPsiFieldMask[(y - p.y() + 160) / 32][(x - p.x() + 256) / 32] )
          return true;
      }
      return false;
    }
    //---------------------------------------------- GENERAL -------------------------------------------------
    template <class _T>
    void swapIfLarger(_T &smaller, _T &larger)
    {
      if ( smaller > larger )
        std::swap(smaller, larger);
    }
    //-------------------------------------------- UNIT FINDER -----------------------------------------------
    template <class finder>
    int getUnitFinderIndex(const finder *uf, int value, int start = 0)
    {
      unsigned int i = start;
      while ( i < 1700*2 && uf[i].searchValue < value && uf[i].unitIndex > 0 )
        ++i;
      return i;
    }
    
    template <class finder>
    void manageUnitFinder(finder *finder_x, finder *finder_y, DWORD *pdwFinderFlags, int left, int top, int right, int bottom, bool (__fastcall *callback)(Unit *uIterator), std::set<Unit*> &finderSet)
    {
      // Clear the set
      finderSet.clear();

      Templates::swapIfLarger<int>(left, right);
      Templates::swapIfLarger<int>(top, bottom);

      // Declare some variables
      int r = right, b = bottom;
      bool isWidthExtended  = right - left + 1 < UnitTypes::maxUnitWidth();
      bool isHeightExtended = top - bottom + 1 < UnitTypes::maxUnitHeight();

      // Check if the location is smaller than the largest unit
      if ( isWidthExtended )
        r += UnitTypes::maxUnitWidth();
      if ( isHeightExtended )
        b += UnitTypes::maxUnitHeight();

      // Obtain finder indexes for all bounds
      int iLeft   = Templates::getUnitFinderIndex<finder>(finder_x, left);
      int iTop    = Templates::getUnitFinderIndex<finder>(finder_y, top);
      int iRight  = Templates::getUnitFinderIndex<finder>(finder_x, r + 1, iLeft);
      int iBottom = Templates::getUnitFinderIndex<finder>(finder_y, b + 1, iTop);

      // Iterate the X entries of the finder
      for ( int x = iLeft; x < iRight; ++x )
      {
        int iUnitIndex = finder_x[x].unitIndex;
        if ( pdwFinderFlags[iUnitIndex] == 1 )
          continue;
        if ( isWidthExtended )
        {
          Unit *u = ((GameImpl*)Broodwar)->_unitFromIndex(iUnitIndex);
          if ( u && u->getLeft() <= right )
            pdwFinderFlags[iUnitIndex] = 1;
        }
        else
          pdwFinderFlags[iUnitIndex] = 1;
      }
      // Iterate the Y entries of the finder
      for ( int y = iTop; y < iBottom; ++y )
      {
        int iUnitIndex = finder_y[y].unitIndex;
        if ( pdwFinderFlags[iUnitIndex] != 1 )
          continue;
        if ( isHeightExtended )
        {
          Unit *u = ((GameImpl*)Broodwar)->_unitFromIndex(iUnitIndex);
          if ( u && u->getTop() <= bottom )
            pdwFinderFlags[iUnitIndex] = 2;
        }
        else
          pdwFinderFlags[iUnitIndex] = 2;
      }
      // Final Iteration
      for ( int x = iLeft; x < iRight; ++x )
      {
        int iUnitIndex = finder_x[x].unitIndex;
        if ( pdwFinderFlags[iUnitIndex] == 2 )
        {
          Unit *u = ((GameImpl*)Broodwar)->_unitFromIndex(iUnitIndex);
          if ( u && u->exists() && callback(u) )
            finderSet.insert(u);
        }
        // Reset finderFlags so it can be reused without incident
        pdwFinderFlags[iUnitIndex] = 0;
      }
    }
    //------------------------------------------- CAN BUILD HERE ---------------------------------------------
    static inline bool canBuildHere(const Unit* builder, TilePosition position, UnitType type, bool checkExplored)
    {
      Broodwar->setLastError(Errors::Unbuildable_Location);
      int width  = type.tileWidth();
      int height = type.tileHeight();

      int left   = position.x();
      int top    = position.y();
      int right  = left + width;
      int bottom = top + height;

      /* Map limit Check */
      if (left < 0) // left
        return false;
      if (top < 0) // top
        return false;
      if (right > Broodwar->mapWidth()) // right
        return false;
      if (bottom >= Broodwar->mapHeight()) // bottom
        return false;

      //if the unit is a refinery, we just need to check the set of geysers to see if the position
      //matches one of them (and the type is still vespene geyser)
      if ( type.isRefinery() )
      {
        foreach (Unit* g, Broodwar->getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
              return false;
            return Broodwar->setLastError(Errors::None);
          }
        }
        return false;
      }

      /* Tile buildability check */
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          // Check if tile is buildable and explored
          if ( !Broodwar->isBuildable(ix, iy, true) || ( checkExplored && !Broodwar->isExplored(ix, iy)) )
            return false; // @TODO: Error code for !isExplored ??
        }
      }

      // Check if builder is capable of reaching the building site
      if ( builder && !builder->getType().isFlagBeacon() && !builder->hasPath( (Position)TilePosition(left + width/2, top + height/2) ) )
        return false;

      /* Ground unit dimension check */
      int targetX = left * 32 + type.tileWidth()  * 32 / 2;
      int targetY = top  * 32 + type.tileHeight() * 32 / 2;
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          foreach(Unit *u, Broodwar->getUnitsOnTile(ix,iy))
          {
            BWAPI::UnitType iterType = u->getType();
            if ( !iterType.isBuilding() &&
                 !iterType.isFlyer()    &&
                 !u->isLoaded()         &&
                 u != builder           &&
                 u->getLeft()    <= targetX + type.dimensionRight()  &&
                 u->getTop()     <= targetY + type.dimensionDown()   &&
                 u->getRight()   >= targetX - type.dimensionLeft()   &&
                 u->getBottom()  >= targetY - type.dimensionUp()  )
            {
              if ( !type.isAddon() )
                return false;
              else if ( !iterType.canMove() )
                return false;
            }
          }
        }
      }

      /* Creep Check */
      if ( type.getRace() == Races::Zerg )
      { // Creep requirement, or ignore creep if there isn't one
        if ( type.requiresCreep() )
          for(int ix = left; ix < right; ++ix)
            for(int iy = top; iy < bottom; ++iy)
              if (!Broodwar->hasCreep(ix, iy))
                return false;
      }
      else
      { // Can't build on the creep
        for(int ix = left; ix < right; ++ix)
          for(int iy = top; iy < bottom; ++iy)
            if (Broodwar->hasCreep(ix, iy))
              return false;
      }

      /* Power Check */
      if ( type.requiresPsi() && !Broodwar->hasPower(left, top, width, height) )
        return false;

      /* Resource Check (CC, Nex, Hatch) */
      if (type.isResourceDepot())
      {
        foreach (BWAPI::Unit* m, Broodwar->getStaticMinerals())
        {
          if (Broodwar->isVisible(m->getInitialTilePosition()) ||
              Broodwar->isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
            if (!m->isVisible())
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (m->getInitialTilePosition().x() > left - 5 &&
              m->getInitialTilePosition().y() > top  - 4 &&
              m->getInitialTilePosition().x() < left + 7 &&
              m->getInitialTilePosition().y() < top  + 6)
            return false;
        }
        foreach (BWAPI::Unit* g, Broodwar->getStaticGeysers())
        {
          if (g->getInitialTilePosition().x() > left - 7 &&
              g->getInitialTilePosition().y() > top  - 5 &&
              g->getInitialTilePosition().x() < left + 7 &&
              g->getInitialTilePosition().y() < top  + 6)
            return false;
        }
      }
      //if the build site passes all these tests, return true.
      return Broodwar->setLastError(Errors::None);
    }
    //------------------------------------------- CAN MAKE ---------------------------------------------------
    static inline bool canMake(const Unit* builder, UnitType type)
    {
      /* Error checking */
      Broodwar->setLastError(Errors::None);
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      BWAPI::UnitType requiredType = type.whatBuilds().first;

      if ( builder )
      {
        /* Check if the owner of the unit is you */
        if (builder->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        BWAPI::UnitType builderType = builder->getType();
        if ( type == UnitTypes::Zerg_Nydus_Canal && builderType == UnitTypes::Zerg_Nydus_Canal )
        {
          if ( !builder->isCompleted() )
            return Broodwar->setLastError(Errors::Unit_Busy);

          if ( builder->getNydusExit() )
            return Broodwar->setLastError(Errors::Unknown);

          return true;
        }

        /* Check if this unit can actually build the unit type */
        if ( requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva() )
        {
          if ( builder->getLarva().size() == 0 )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
        else if ( builderType != requiredType )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        /* Carrier space */
        if ( builderType == UnitTypes::Protoss_Carrier )
        {
          int max_amt = 4;
          if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0)
            max_amt += 4;
          if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
        else if ( builderType == UnitTypes::Hero_Gantrithor )
        {
          if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= 8)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }

        /* Reaver Space */
        if ( builderType == UnitTypes::Protoss_Reaver )
        {
          int max_amt = 5;
          if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
            max_amt += 5;
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
        else if ( builderType == UnitTypes::Hero_Warbringer )
        {
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= 10)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
      } // builder

      /* Check if player has enough minerals */
      if ( Broodwar->self()->minerals() < type.mineralPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      /* Check if player has enough gas */
      if ( Broodwar->self()->gas() < type.gasPrice() )
        return Broodwar->setLastError(Errors::Insufficient_Gas);
      
      /* Check if player has enough supplies */
      BWAPI::Race typeRace = type.getRace();
      if ( type.supplyRequired() > 0 && Broodwar->self()->supplyTotal(typeRace) < Broodwar->self()->supplyUsed(typeRace) + type.supplyRequired() - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0) )
        return Broodwar->setLastError(Errors::Insufficient_Supply);

      UnitType addon = UnitTypes::None;
      std::map<UnitType, int>::const_iterator requiredEnd = type.requiredUnits().end();
      for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != requiredEnd; ++i)
      {
        if (i->first.isAddon())
          addon = i->first;

        bool pass = false;
        if (Broodwar->self()->completedUnitCount(i->first) >= i->second)
          pass = true;
        if ( i->first == UnitTypes::Zerg_Hatchery &&
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hatchery) +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Lair)     +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hive)     >= i->second )
          pass = true;
        if ( i->first == UnitTypes::Zerg_Lair && 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Lair) + 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
        if ( i->first == UnitTypes::Zerg_Spire && 
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Spire) +
             Broodwar->self()->completedUnitCount(UnitTypes::Zerg_Greater_Spire) >= i->second )
          pass = true;
        
        if ( !pass )
          return Broodwar->setLastError(Errors::Insufficient_Tech);
      }

      if (type.requiredTech() != TechTypes::None && !Broodwar->self()->hasResearched(type.requiredTech()))
        return Broodwar->setLastError(Errors::Insufficient_Tech);

      if ( builder && 
           addon != UnitTypes::None &&
           addon.whatBuilds().first == type.whatBuilds().first &&
           (!builder->getAddon() || builder->getAddon()->getType() != addon) )
        return Broodwar->setLastError(Errors::Insufficient_Tech);
      return true;
    }
    //------------------------------------------- CAN RESEARCH -----------------------------------------------
    static inline bool canResearch(const Unit* unit, TechType type)
    {
      /* Error checking */
      Broodwar->setLastError(Errors::None);
      if ( !Broodwar->self() )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( unit )
      {
        if (unit->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (unit->getType() != type.whatResearches())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( unit->isLifted() || !unit->isIdle() || !unit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      if (Broodwar->self()->isResearching(type))
        return Broodwar->setLastError(Errors::Currently_Researching);

      if (Broodwar->self()->hasResearched(type))
        return Broodwar->setLastError(Errors::Already_Researched);

      if (Broodwar->self()->minerals() < type.mineralPrice())
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if (Broodwar->self()->gas() < type.gasPrice())
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      return true;
    }
    //------------------------------------------- CAN UPGRADE ------------------------------------------------
    static inline bool canUpgrade(const Unit* unit, UpgradeType type)
    {
      Broodwar->setLastError(Errors::None);
      Player *self = Broodwar->self();
      if ( !self )
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if ( unit )
      {
        if (unit->getPlayer() != self)
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if (unit->getType() != type.whatUpgrades())
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( unit->isLifted() || !unit->isIdle() || !unit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }
      int nextLvl = self->getUpgradeLevel(type)+1;
      
      UnitType what = type.whatUpgrades();
      if ( what != UnitTypes::None )
      {
        if ( what == UnitTypes::Zerg_Hatchery && 
             !self->completedUnitCount(UnitTypes::Zerg_Hatchery) &&
             !self->completedUnitCount(UnitTypes::Zerg_Lair) &&
             !self->completedUnitCount(UnitTypes::Zerg_Hive) )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        else if ( what == UnitTypes::Zerg_Lair &&
                  !self->completedUnitCount(UnitTypes::Zerg_Lair) &&
                  !self->completedUnitCount(UnitTypes::Zerg_Hive) )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        else if ( what == UnitTypes::Zerg_Spire &&
                  !self->completedUnitCount(UnitTypes::Zerg_Spire) &&
                  !self->completedUnitCount(UnitTypes::Zerg_Greater_Spire) )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        else if ( !self->completedUnitCount(what) )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
      }
      
      UnitType req = type.whatsRequired(nextLvl);
      if ( req != UnitTypes::None )
      {
        if ( req == UnitTypes::Zerg_Hatchery && !self->completedUnitCount(UnitTypes::Zerg_Hatchery) )
        {
          if ( !self->allUnitCount(UnitTypes::Zerg_Lair) &&
               !self->allUnitCount(UnitTypes::Zerg_Hive) )
            return Broodwar->setLastError(Errors::Insufficient_Tech);
        }
        else if ( req == UnitTypes::Zerg_Lair && !self->completedUnitCount(UnitTypes::Zerg_Lair) )
        {
          if ( !self->allUnitCount(UnitTypes::Zerg_Hive) )
            return Broodwar->setLastError(Errors::Insufficient_Tech);
        }
        else if ( req == UnitTypes::Zerg_Spire && !self->completedUnitCount(UnitTypes::Zerg_Spire) )
        {
          if ( !self->allUnitCount(UnitTypes::Zerg_Greater_Spire) )
            return Broodwar->setLastError(Errors::Insufficient_Tech);
        }
        else if ( !self->completedUnitCount(req) )
          return Broodwar->setLastError(Errors::Insufficient_Tech);
      }

      if (self->isUpgrading(type))
        return Broodwar->setLastError(Errors::Currently_Upgrading);

      if (self->getUpgradeLevel(type) >= type.maxRepeats())
        return Broodwar->setLastError(Errors::Fully_Upgraded);

      if ( self->minerals() < type.mineralPrice(nextLvl) )
        return Broodwar->setLastError(Errors::Insufficient_Minerals);

      if ( self->gas() < type.gasPrice(nextLvl) )
        return Broodwar->setLastError(Errors::Insufficient_Gas);

      return true;
    }

    //------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
    static inline bool canIssueCommand(const Unit* thisUnit, UnitCommand c)
    {
      c.unit = (Unit*)thisUnit;
      BWAPI::UnitCommandType ct = c.type;
      // train/morph helper, get first larva
      if (UnitCommandTypes::Train == ct ||
          UnitCommandTypes::Morph == ct)
      {
        if (thisUnit->getType().producesLarva() && c.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          if (thisUnit->getLarva().empty())
          {
            Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
            return false;
          }
          c.unit = (UnitImpl*)(*thisUnit->getLarva().begin());
          thisUnit = c.unit;
        }
      }

      // Basic header
      Broodwar->setLastError(Errors::None);
      if (thisUnit->getPlayer() != Broodwar->self())
        return Broodwar->setLastError(Errors::Unit_Not_Owned);

      if (!thisUnit->exists())
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

      // Global can be ordered check
      if ( thisUnit->isLockedDown() || 
           thisUnit->isMaelstrommed() || 
           thisUnit->isStasised()  || 
           thisUnit->isUnpowered() ||
           thisUnit->getOrder() == Orders::ZergBirth ||
           thisUnit->isLoaded() )
        return Broodwar->setLastError(Errors::Unit_Busy);

      // Hallucination check
      if ( thisUnit->isHallucination()                  &&
           UnitCommandTypes::Attack_Move          != ct &&
           UnitCommandTypes::Attack_Unit          != ct &&
           UnitCommandTypes::Move                 != ct &&
           UnitCommandTypes::Patrol               != ct &&
           UnitCommandTypes::Hold_Position        != ct &&
           UnitCommandTypes::Stop                 != ct &&
           UnitCommandTypes::Follow               != ct &&
           UnitCommandTypes::Load                 != ct &&
           UnitCommandTypes::Right_Click_Position != ct &&
           UnitCommandTypes::Right_Click_Unit     != ct )
         return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // Can be ordered check
      if ( !thisUnit->getType().isBuilding() &&
           !thisUnit->isInterruptible() &&
           (UnitCommandTypes::Attack_Unit           == ct ||
            UnitCommandTypes::Attack_Move           == ct ||
            UnitCommandTypes::Build                 == ct ||
            UnitCommandTypes::Follow                == ct ||
            UnitCommandTypes::Gather                == ct ||
            UnitCommandTypes::Load                  == ct ||
            UnitCommandTypes::Move                  == ct ||
            UnitCommandTypes::Patrol                == ct ||
            UnitCommandTypes::Repair                == ct ||
            UnitCommandTypes::Return_Cargo          == ct ||
            UnitCommandTypes::Right_Click_Position  == ct ||
            UnitCommandTypes::Right_Click_Unit      == ct ||
            UnitCommandTypes::Unload                == ct ||
            UnitCommandTypes::Unload_All            == ct ||
            UnitCommandTypes::Unload_All_Position   == ct ||
            UnitCommandTypes::Use_Tech              == ct ||
            UnitCommandTypes::Use_Tech_Unit         == ct ||
            UnitCommandTypes::Use_Tech_Position     == ct ) )
        return Broodwar->setLastError(Errors::Unit_Busy);

      // valid target check
      if ((!c.target ||
          !c.target->exists()) &&
          (UnitCommandTypes::Attack_Unit      == ct ||
           UnitCommandTypes::Set_Rally_Unit   == ct ||
           UnitCommandTypes::Follow           == ct ||
           UnitCommandTypes::Gather           == ct ||
           UnitCommandTypes::Repair           == ct ||
           UnitCommandTypes::Load             == ct ||
           UnitCommandTypes::Unload           == ct ||
           UnitCommandTypes::Right_Click_Unit == ct ||
           UnitCommandTypes::Use_Tech_Unit    == ct) )
        return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

      // Attack Unit requirements
      if ( UnitCommandTypes::Attack_Unit == ct || 
          (UnitCommandTypes::Right_Click_Unit == ct && !c.target->getPlayer()->isNeutral() && thisUnit->getPlayer()->isEnemy(c.target->getPlayer())))
      {
        WeaponType weapon = thisUnit->getType().groundWeapon();
        bool targetInAir = (c.target->isLifted() || c.target->getType().isFlyer());
        if (targetInAir)
          weapon = thisUnit->getType().airWeapon();

        bool canAttack = (weapon != WeaponTypes::None);

        if ( ( (thisUnit->getType() == UnitTypes::Protoss_Reaver || thisUnit->getType() == UnitTypes::Hero_Warbringer) && thisUnit->getScarabCount() > 0 && !targetInAir) || 
          ((thisUnit->getType() == UnitTypes::Protoss_Carrier    || thisUnit->getType() == UnitTypes::Hero_Gantrithor) && thisUnit->getInterceptorCount() > 0) )
          canAttack = true;

        if (!canAttack)
          return Broodwar->setLastError(Errors::Unable_To_Hit);

        if ( !thisUnit->getType().canMove() && !thisUnit->isInWeaponRange(c.target) )
          return Broodwar->setLastError(Errors::Out_Of_Range);
      }

      // Build/Train requirements
      if ( UnitCommandTypes::Build       == ct)
      {
        if (TilePosition(c.x,c.y).isValid()==false)
          return Broodwar->setLastError(Errors::Invalid_Tile_Position);
      }

      if ( UnitCommandTypes::Morph       == ct ||
           UnitCommandTypes::Train       == ct)
      {
        UnitType uType = c.getUnitType();
        if ( thisUnit->getType().producesLarva() && uType.whatBuilds().first == UnitTypes::Zerg_Larva )
        {
          if (thisUnit->getLarva().empty())
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
          UnitImpl *larva = (UnitImpl*)(*thisUnit->getLarva().begin());
          return canIssueCommand( larva, UnitCommand::train(larva,uType) );
        }
      }
      if ( UnitCommandTypes::Build       == ct ||
           UnitCommandTypes::Build_Addon == ct ||
           UnitCommandTypes::Morph       == ct ||
           UnitCommandTypes::Train       == ct )
      {
        UnitType uType = c.getUnitType();
        if ( !Broodwar->canMake(thisUnit, uType) )
          return false;

        if ( thisUnit->isConstructing() || 
             !thisUnit->isCompleted()   || 
             (thisUnit->getType().isFlyingBuilding() && thisUnit->isLifted()) ||
             (UnitCommandTypes::Train != ct && thisUnit->getType().isBuilding() && !thisUnit->isIdle()) )
          return Broodwar->setLastError(Errors::Unit_Busy);

        if ( UnitCommandTypes::Build == ct )
        {
          if ( !uType.isBuilding() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( !uType.isAddon() && !Broodwar->canBuildHere(thisUnit, BWAPI::TilePosition(c.x, c.y), uType, true) )
            return false;
        }
        else if ( UnitCommandTypes::Build_Addon == ct )
        {
          if ( !uType.isAddon() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( thisUnit->getAddon() )
            return Broodwar->setLastError(Errors::Incompatible_State);

          if ( !Broodwar->canBuildHere(thisUnit, BWAPI::TilePosition(thisUnit->getTilePosition().x() + 4, thisUnit->getTilePosition().y() + 1), uType) )
            return false;
        }
        else
        {
          if ( thisUnit->getType().producesLarva() && uType.whatBuilds().first == UnitTypes::Zerg_Larva && thisUnit->getLarva().size() == 0 )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        }
      } // build/train

      // Research/Upgrade requirements
      if ( UnitCommandTypes::Research == ct && !Broodwar->canResearch(thisUnit, c.getTechType()) )
        return false;
      if ( UnitCommandTypes::Upgrade  == ct && !Broodwar->canUpgrade(thisUnit, c.getUpgradeType()) )
        return false;

      // Set Rally 
      if ( (UnitCommandTypes::Set_Rally_Position == ct || UnitCommandTypes::Set_Rally_Unit == ct) && !thisUnit->getType().canProduce() )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      /* Commented out because it breaks RallyTest.
      if ( UnitCommandTypes::Set_Rally_Position == ct && thisUnit->getRallyPosition().x() == c.x && thisUnit->getRallyPosition().y() == c.y )
        return false;

      if ( UnitCommandTypes::Set_Rally_Unit == ct && thisUnit == c.target )
        return false;
      */
      // Move/stop/standard
      if ( (UnitCommandTypes::Move          == ct || 
            UnitCommandTypes::Patrol        == ct ||
            UnitCommandTypes::Hold_Position == ct ||
            UnitCommandTypes::Stop          == ct ||
            UnitCommandTypes::Follow        == ct) &&
            thisUnit->getType().isBuilding() &&
            !thisUnit->isLifted() &&
            ct != UnitCommandTypes::Hold_Position &&
            ct != UnitCommandTypes::Stop )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // Gather
      if ( UnitCommandTypes::Gather == ct )
      {
        UnitType uType = c.target->getType();
        if ( !thisUnit->getType().isWorker() ||
             !uType.isResourceContainer()    ||
             uType == UnitTypes::Resource_Vespene_Geyser )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->getPowerUp() || !c.target->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);
      } // gather

      // return cargo
      if ( UnitCommandTypes::Return_Cargo == ct )
      {
        if ( !thisUnit->getType().isWorker() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( !thisUnit->isCarryingGas() && !thisUnit->isCarryingMinerals() )
          return Broodwar->setLastError(Errors::Insufficient_Ammo);
      } // return

      // Repair
      if ( UnitCommandTypes::Repair == ct )
      {
        UnitType targType = c.target->getType();
        if ( thisUnit->getType() != BWAPI::UnitTypes::Terran_SCV ||
             targType.getRace()  != BWAPI::Races::Terran         ||
             !targType.isMechanical() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
      } // repair

      // Ability requirements
      if ( UnitCommandTypes::Use_Tech          == ct ||
           UnitCommandTypes::Use_Tech_Position == ct ||
           UnitCommandTypes::Use_Tech_Unit     == ct ||
           UnitCommandTypes::Burrow            == ct ||
           UnitCommandTypes::Cloak             == ct ||
           UnitCommandTypes::Siege             == ct )
      {
        // Retrieve the tech type
        BWAPI::TechType tech = TechType(c.extra);
        if ( UnitCommandTypes::Use_Tech               == ct)
        {
          if (tech.targetsUnit() || tech.targetsPosition() || tech == TechTypes::None || tech == TechTypes::Unknown || tech == TechTypes::Lurker_Aspect)
            return Broodwar->setLastError(Errors::Incompatible_TechType);
        }
        else if ( UnitCommandTypes::Use_Tech_Position == ct)
        {
          if ( !tech.targetsPosition() )
            return Broodwar->setLastError(Errors::Incompatible_TechType);
        }
        else if ( UnitCommandTypes::Use_Tech_Unit     == ct)
        {
          if ( !tech.targetsUnit() )
            return Broodwar->setLastError(Errors::Incompatible_TechType);
        }
        if ( UnitCommandTypes::Burrow == ct )
          tech = BWAPI::TechTypes::Burrowing;
        else if ( UnitCommandTypes::Cloak == ct )
          tech = thisUnit->getType().cloakingTech();
        else if ( UnitCommandTypes::Siege == ct )
          tech = BWAPI::TechTypes::Tank_Siege_Mode;

        // researched check
        if ( !thisUnit->getType().isHero() && !Broodwar->self()->hasResearched(tech) && thisUnit->getType() != UnitTypes::Zerg_Lurker )
          return Broodwar->setLastError(Errors::Insufficient_Tech);

        // energy check
        if ( thisUnit->getEnergy() < tech.energyUsed() )
          return Broodwar->setLastError(Errors::Insufficient_Energy);

        // unit check
        if ( tech != TechTypes::Burrowing && tech.whatUses().find(thisUnit->getType()) == tech.whatUses().end() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        // spider mine check
        if ( tech == TechTypes::Spider_Mines && thisUnit->getSpiderMineCount() <= 0 )
          return Broodwar->setLastError(Errors::Insufficient_Ammo);

        // nuclear missile check
        if ( tech == TechTypes::Nuclear_Strike && thisUnit->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) == 0 )
          return Broodwar->setLastError(Errors::Insufficient_Ammo);

        // state checks
        if (tech == TechTypes::Burrowing)
        {
          if ( !thisUnit->getType().isBurrowable() )
            return Broodwar->setLastError(Errors::Incompatible_UnitType);

          if ( thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Burrowing || thisUnit->getOrder() == Orders::Unburrowing )
            return Broodwar->setLastError(Errors::Incompatible_State);
        }
        else if (tech == TechTypes::Tank_Siege_Mode)
        {
          if ( thisUnit->isSieged() )
            return Broodwar->setLastError(Errors::Incompatible_State);

          if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
            return Broodwar->setLastError(Errors::Unit_Busy);
        }
        else if (tech == TechTypes::Personnel_Cloaking || tech == TechTypes::Cloaking_Field)
        {
          if ( thisUnit->getSecondaryOrder() == Orders::Cloak )
            return Broodwar->setLastError(Errors::Incompatible_State);
        }

        if ( tech == TechTypes::None || tech == TechTypes::Unknown )
          return Broodwar->setLastError(Errors::Incompatible_TechType);
      } // ability

      // Unburrow
      if ( UnitCommandTypes::Unburrow == ct )
      {
        if ( !thisUnit->getType().isBurrowable() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        if ( !thisUnit->isBurrowed() || thisUnit->getOrder() == Orders::Unburrowing )
          return Broodwar->setLastError(Errors::Incompatible_State);
      } // unburrow

      // Decloak
      if ( UnitCommandTypes::Decloak == ct )
      {
        if ( thisUnit->getType().cloakingTech() == TechTypes::None )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->getSecondaryOrder() != Orders::Cloak )
          return Broodwar->setLastError(Errors::Incompatible_State);
      } // decloak

      // Unsiege
      if ( UnitCommandTypes::Unsiege == ct )
      {
        if ( !thisUnit->isSieged() )
          return Broodwar->setLastError(Errors::Incompatible_State);

        if ( thisUnit->getOrder() == Orders::Sieging || thisUnit->getOrder() == Orders::Unsieging )
          return Broodwar->setLastError(Errors::Unit_Busy);
      }

      // lift/land
      if ( UnitCommandTypes::Lift == ct || UnitCommandTypes::Land == ct )
      {
        if ( !thisUnit->getType().isFlyingBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( UnitCommandTypes::Lift == ct ? thisUnit->isLifted() : !thisUnit->isLifted() )
          return Broodwar->setLastError(Errors::Incompatible_State);
      } // lift/land

      // load
      if ( UnitCommandTypes::Load == ct)
      {
        //target must also be owned by self
        if (c.target->getPlayer() != Broodwar->self())
          return Broodwar->setLastError(Errors::Unit_Not_Owned);

        if ( c.target->isLoaded() ||
             !c.target->isCompleted() ||
             !thisUnit->isCompleted() )
          return Broodwar->setLastError(Errors::Unit_Busy);

        // verify upgrade for Zerg Overlord
        int thisUnitSpaceProvided = thisUnit->getType().spaceProvided();
        if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
          return Broodwar->setLastError(Errors::Insufficient_Tech);

        int targetSpaceProvided = c.target->getType().spaceProvided();
        if ( c.target->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
          return Broodwar->setLastError(Errors::Insufficient_Tech);

        // check if there is space available
        int thisUnitFreeSpace = thisUnitSpaceProvided;
        int targetFreeSpace   = targetSpaceProvided;
        std::set<Unit*> loadedUnits = thisUnit->getLoadedUnits();
        foreach(Unit* u, loadedUnits)
        {
          int r = u->getType().spaceRequired();
          if (r > 0 && r < 8)
            thisUnitFreeSpace -= r;
        }
        std::set<Unit*> targetLoadedUnits = c.target->getLoadedUnits();
        foreach(Unit* u, targetLoadedUnits)
        {
          int r = u->getType().spaceRequired();
          if (r > 0 && r < 8)
            targetFreeSpace -= r;
        }
        if (thisUnitSpaceProvided > 0)
        {
          if (c.target->getType().canMove() == false || c.target->getType().isFlyer() || c.target->getType().spaceRequired() > 8)
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if (c.target->getType().spaceRequired() > thisUnitFreeSpace)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
        else if (targetSpaceProvided > 0)
        {
          if (thisUnit->getType().canMove() == false || thisUnit->getType().isFlyer() || thisUnit->getType().spaceRequired() > 8)
            return Broodwar->setLastError(Errors::Incompatible_UnitType);
          if (thisUnit->getType().spaceRequired() > targetFreeSpace)
            return Broodwar->setLastError(Errors::Insufficient_Space);
        }
        else
        {
          return Broodwar->setLastError(Errors::Incompatible_UnitType);
        }
      }

      // unload
      if ( UnitCommandTypes::Unload              == ct ||
           UnitCommandTypes::Unload_All          == ct ||
           UnitCommandTypes::Unload_All_Position == ct )
      {
        if ( thisUnit->getLoadedUnits().size() == 0 )
          return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

        // Check overlord tech
        int thisUnitSpaceProvided = thisUnit->getType().spaceProvided();
        if ( thisUnit->getType() == UnitTypes::Zerg_Overlord && Broodwar->self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
          return Broodwar->setLastError(Errors::Insufficient_Tech);

        if ( thisUnitSpaceProvided <= 0 )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( UnitCommandTypes::Unload == ct )
        {
          if ( !c.target )
            return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
          if ( !c.target->isLoaded() )
            return Broodwar->setLastError(Errors::Incompatible_State);
        }

        if ( thisUnit->getType() != UnitTypes::Terran_Bunker && thisUnit )
        {
          BWAPI::Position targDropPos = (UnitCommandTypes::Unload_All_Position == ct ? c.getTargetPosition() : thisUnit->getPosition());
          if ( !Broodwar->isWalkable(targDropPos.x()/8, targDropPos.y()/8) )
            return Broodwar->setLastError(Errors::Unreachable_Location);
        }
      } // unload

      // Halt construction
      if ( UnitCommandTypes::Halt_Construction == ct && thisUnit->getOrder() != Orders::ConstructingBuilding )
        return false;

      // Cancel construction
      if ( UnitCommandTypes::Cancel_Construction == ct )
      {
        if ( !thisUnit->getType().isBuilding() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( thisUnit->isCompleted() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit()) )
          return false;
      }

      // cancel addon
      if ( UnitCommandTypes::Cancel_Addon == ct && (!thisUnit->getAddon() || thisUnit->getAddon()->isCompleted()) )
        return Broodwar->setLastError(Errors::Incompatible_UnitType);

      // cancel train
      if ( UnitCommandTypes::Cancel_Train == ct && !thisUnit->isTraining() )
        return false;

      if ( UnitCommandTypes::Cancel_Train_Slot == ct && (!thisUnit->isTraining() || (thisUnit->getTrainingQueue().size() <= (unsigned int)c.extra && c.extra >= 0)) )
        return false;

      // cancel morph
      if ( UnitCommandTypes::Cancel_Morph == ct && (!thisUnit->isMorphing() || (!thisUnit->isCompleted() && thisUnit->getType() == UnitTypes::Zerg_Nydus_Canal && thisUnit->getNydusExit())) )
        return false;

      // cancel research
      if ( UnitCommandTypes::Cancel_Research == ct && thisUnit->getOrder() != Orders::ResearchTech )
        return false;

      // cancel upgrade
      if ( UnitCommandTypes::Cancel_Upgrade == ct && thisUnit->getOrder() != Orders::Upgrade )
        return false;

      // place COP
      if ( UnitCommandTypes::Place_COP == ct )
      {
        if ( !thisUnit->getType().isFlagBeacon() )
          return Broodwar->setLastError(Errors::Incompatible_UnitType);

        if ( ((UnitImpl*)thisUnit)->self->buttonset == 228 || thisUnit->getOrder() != BWAPI::Orders::CTFCOPInit )
          return Broodwar->setLastError(Errors::Incompatible_State);

        if ( !Broodwar->canBuildHere(thisUnit, BWAPI::TilePosition(c.x, c.y), thisUnit->getType(), true) )
          return false;
      }
      return true;
    }
  }
  //--------------------------------------------- COMPUTE DISTANCE -------------------------------------------
  static inline int computeDistance(const Unit *src, const Unit *targ)
  {
    if ( src == targ || !src || !targ )
      return 0;

    int xDist = src->getLeft() - (targ->getRight() + 1);
    if ( xDist < 0 )
    {
      xDist = targ->getLeft() - (src->getRight() + 1);
      if ( xDist < 0 )
        xDist = 0;
    }
    int yDist = src->getTop() - (targ->getBottom() + 1);
    if ( yDist < 0 )
    {
      yDist = targ->getTop() - (src->getBottom() + 1);
      if ( yDist < 0 )
        yDist = 0;
    }
    return Position(0, 0).getApproxDistance(Position(xDist, yDist));
  }
  //--------------------------------------------- COMPUTE DISTANCE -------------------------------------------
  static inline int computeDistance(const Unit* src, Position targ)
  {
    if ( !src )
      return 0;

    int xDist = src->getLeft() - (targ.x() + 1);
    if ( xDist < 0 )
    {
      xDist = targ.x() - (src->getRight() + 1);
      if ( xDist < 0 )
        xDist = 0;
    }
    int yDist = src->getTop() - (targ.y() + 1);
    if ( yDist < 0 )
    {
      yDist = targ.y() - (src->getBottom() + 1);
      if ( yDist < 0 )
        yDist = 0;
    }
    return Position(0, 0).getApproxDistance(Position(xDist, yDist));
  }
}