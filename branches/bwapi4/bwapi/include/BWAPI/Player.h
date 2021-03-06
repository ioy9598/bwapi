#pragma once
#include <string>

#include <BWAPI/Position.h>
#include <BWAPI/Race.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Interface.h>

namespace BWAPI
{
  // Forwards
  class Color;
  class Force;
  class PlayerType;
  class TechType;
  class Unit;
  class Unitset;
  class UnitType;
  class UpgradeType;
  class WeaponType;

  struct PlayerData;

  /// The Player represents a unique controller in the game. Each player in a match will have his
  /// or her own player instance. There is also a neutral player which owns all the neutral units
  /// (such as mineral patches and vespene geysers).
  ///
  /// @see Playerset, PlayerType, Race
  class Player : public Interface<Player>
  {
  protected:
    virtual ~Player() {};
  public :
    /// Retrieves a unique ID that represents the player.
    ///
    /// @returns
    ///   An integer representing the ID of the player.
    virtual int getID() const = 0;

    /// Retrieves the name of the player.
    ///
    /// @returns
    ///   A std::string object containing the player's name.
    ///
    /// @note Don't forget to use std::string::c_str() when passing this parameter to
    /// Game::sendText and other variadic functions.
    ///
    /// Example usage:
    /// @code
    ///   BWAPI::Player *myEnemy = BWAPI::Broodwar->enemy();
    ///   if ( myEnemy != nullptr )   // Make sure there is an enemy!
    ///     BWAPI::Broodwar->sendText("Prepare to be crushed, %s!", myEnemy->getName().c_str());
    /// @endcode
    virtual std::string getName() const = 0;

    /// Retrieves the set of all units that the player owns. This also includes incomplete units.
    ///
    /// @returns Reference to a Unitset containing the units.
    ///
    /// @note This does not include units that are loaded into transports, @Bunkers, @Refineries,
    /// @Assimilators, or @Extractors.
    virtual const Unitset &getUnits() const = 0;

    /// Retrieves the race of the player. This allows you to change strategies against different
    /// races, or generalize some commands for yourself.
    ///
    /// @retval Races::Unknown If the player chose Races::Random when the game started and they
    /// have not been seen.
    ///
    /// @returns
    ///   The Race that the player is using.
    ///
    /// Example usage:
    /// @code
    ///   if ( BWAPI::Broodwar->enemy() )
    ///   {
    ///     BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();
    ///     if ( enemyRace == Races::Zerg )
    ///       BWAPI::Broodwar->sendText("Do you really think you can beat me with a zergling rush?");
    ///   }
    /// @endcode
    virtual Race getRace() const = 0;

    /// Retrieves the player's controller type. This allows you to distinguish betweeen computer
    /// and human players.
    ///
    /// @returns
    ///   The PlayerType that identifies who is controlling a player.
    ///
    /// @note Other players using BWAPI will be treated as a human player and return
    /// PlayerTypes::Player.
    ///
    /// @code
    ///   if ( BWAPI::Broodwar->enemy() )
    ///   {
    ///     if ( BWAPI::Broodwar->enemy()->getType() == PlayerTypes::Computer )
    ///       BWAPI::Broodwar << "Looks like something I can abuse!" << std::endl;
    ///   }
    /// @endcode
    virtual PlayerType getType() const = 0;

    /// Retrieves the player's force. A force is the team that the player is playing on. This is
    /// only used in non-melee game types.
    ///
    /// @returns
    ///   The Force object that the player is part of.
    virtual Force* getForce() const = 0;

    /// Checks if this player is allied to the specified player.
    ///
    /// @param player
    ///     The player to check alliance with.
    ///
    /// @retval true if this player is allied with \p player .
    /// @retval false if this player is not allied with \p player.
    ///
    /// @note This function will also return false if this player is neutral or an observer, or
    /// if \p player is neutral or an observer.
    ///
    /// @see isEnemy
    virtual bool isAlly(Player* player) const = 0;

    /// Checks if this player is unallied to the specified player.
    ///
    /// @param player
    ///     The player to check alliance with.
    ///
    /// @retval true if this player is allied with \p player .
    /// @retval false if this player is not allied with \p player .
    ///
    /// @note This function will also return false if this player is neutral or an observer, or if
    /// \p player is neutral or an observer.
    ///
    /// @see isAlly
    virtual bool isEnemy(Player* player) const = 0;

    /// Checks if this player is the neutral player.
    ///
    /// @retval true if this player is the neutral player.
    /// @retval false if this player is any other player.
    virtual bool isNeutral() const = 0;

    /// Retrieve's the player's starting location.
    ///
    /// @returns
    ///   A TilePosition containing the position of the start location.
    ///
    /// @retval TilePositions::None if the player does not have a start location.
    /// @retval TilePositions::Unknown if an error occured while trying to retrieve the start
    /// location.
    ///
    /// @see Game::getStartLocations, Game::getLastError
    virtual TilePosition getStartLocation() const = 0;

    /** Returns true if the player has achieved victory. */
    virtual bool isVictorious() const = 0;

    /** Returns true if the player has been defeated. */
    virtual bool isDefeated() const = 0;

    /** Returns true if the player left the game. */
    virtual bool leftGame() const = 0;

    /** Returns the amount of minerals the player has. */
    virtual int minerals() const = 0;

    /** Returns the amount of vespene gas the player has. */
    virtual int gas() const = 0;

    /** Returns the cumulative amount of minerals the player has mined up to this point (including the 50
     * minerals at the start of the game). */
    virtual int gatheredMinerals() const = 0;

    /** Returns the cumulative amount of gas the player has harvested up to this point. */
    virtual int gatheredGas() const = 0;

    /** Returns the cumulative amount of minerals the player has spent on repairs up to this point. */
    virtual int repairedMinerals() const = 0;

    /** Returns the cumulative amount of gas the player has spent on repairs up to this point. */
    virtual int repairedGas() const = 0;

    /** Returns the cumulative amount of minerals the player has gained from refunded units up to this point. */
    virtual int refundedMinerals() const = 0;

    /** Returns the cumulative amount of gas the player has gained from refunded units up to this point. */
    virtual int refundedGas() const = 0;

    /** Returns the cumulative amount of minerals the player has spent up to this point (not including repairs). */
    virtual int spentMinerals() const = 0;

    /** Returns the cumulative amount of gas the player has spent up to this point (not including repairs). */
    virtual int spentGas() const = 0;

    /** Returns the total amount of supply the player has. If a race is provided, the total supply for the
     * given race will be returned, otherwise the player's initial race will be used. Supply counts returned
     * by BWAPI are double what you would expect to see from playing the game. This is because zerglings
     * take up 0.5 in-game supply. */
    virtual int supplyTotal(Race race = Races::None) const = 0;

    /** Returns how much of the supply is actually being used by units. If a race is provided, the used
     * supply for the given race will be returned, otherwise the player's initial race will be used. Supply
     * counts returned by BWAPI are double what you would expect to see from playing the game. This is
     * because zerglings take up 0.5 in-game supply. */
    virtual int supplyUsed(Race race = Races::None) const = 0;

    /** Returns the number of all accessible units of the given type. */
    virtual int allUnitCount(UnitType unit) const = 0;

    /** Returns the number of visible units of the given type. */
    virtual int visibleUnitCount(UnitType unit) const = 0;

    /** Returns the number of completed units of the given type. */
    virtual int completedUnitCount(UnitType unit) const = 0;

    /** Returns the number of incomplete units of the given type. */
    int incompleteUnitCount(UnitType unit) const;

    /** Returns the number of dead units of the given type. */
    virtual int deadUnitCount(UnitType unit) const = 0;

    /** Returns the number of killed units of the given type. */
    virtual int killedUnitCount(UnitType unit) const = 0;

    /** Returns the player's current upgrade level of the given upgrade. To order a unit to upgrade a given
     * upgrade type, see Unit::upgrade. */
    virtual int getUpgradeLevel(UpgradeType upgrade) const = 0;

    /** Returns true if the player has finished researching the given tech. To order a unit to research a
     * given tech type, see Unit::research. */
    virtual bool hasResearched(TechType tech) const = 0;

    /** Returns true if the player is researching the given tech. To order a unit to research a given tech
     * type, see Unit::reseach. */
    virtual bool isResearching(TechType tech) const = 0;

    /** Returns true if the player is upgrading the given upgrade. To order a unit to upgrade a given
     * upgrade type, see Unit::upgrade. */
    virtual bool isUpgrading(UpgradeType upgrade) const = 0;

    /** Returns the color of the player for drawing */
    virtual BWAPI::Color getColor() const = 0;

    /** Returns the color of the player for text messages */
    char getTextColor() const;

    /** Returns the max energy of the given unit type, taking into account upgrades */
    int maxEnergy(UnitType unit) const;

    /** Returns the top speed of the given unit type, includes upgrades */
    double topSpeed(UnitType unit) const;

    /** Returns the max range of the given weapon with upgrades */
    int weaponMaxRange(WeaponType weapon) const;

    /** Returns the sight range of the given unit type, includes upgrades */
    int sightRange(UnitType unit) const;

    /** Returns the ground weapon cooldown of the given unit type, includes upgrades */
    int groundWeaponDamageCooldown(UnitType unit) const;

    /// Calculates the armor that a given unit type will have, including upgrades.
    ///
    /// @param unit
    ///   The unit type to calculate armor for, using the current player's upgrades.
    ///
    /// @returns The amount of armor that the unit will have with the player's upgrades.
    int armor(UnitType unit) const;

    /// Calculates the damage that a given weapon type can deal, including upgrades.
    ///
    /// @param wpn
    ///   The weapon type to calculate for.
    ///
    /// @returns The amount of damage that the weapon deals with this player's upgrades.
    int damage(WeaponType wpn) const;

    /** Returns the Player's Total Unit Score */
    virtual int getUnitScore() const = 0;

    /** Returns the Player's Total Kill Score */
    virtual int getKillScore() const = 0;

    /** Returns the Player's Total Buildings Score */
    virtual int getBuildingScore() const = 0;

    /** Returns the Player's Total Razings Score */
    virtual int getRazingScore() const = 0;

    /** Returns the Player's Custom Score */
    virtual int getCustomScore() const = 0;

    /** Returns true if the Player is only observing the game, and not participating */
    virtual bool isObserver() const = 0;

    /** Returns the maximum upgrades available specific to the player (Use Map Settings). */
    virtual int getMaxUpgradeLevel(UpgradeType upgrade) const = 0;

    /** Returns true if the research is available for the player to research (Use Map Settings). */
    virtual bool isResearchAvailable(TechType tech) const = 0;

    /** Returns true if the unit is available for the player to build (Use Map Settings). */
    virtual bool isUnitAvailable(UnitType unit) const = 0;
  };
};
