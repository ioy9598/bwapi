#include <Util/Types.h>

namespace BW
{
  /**
   * Enumeration of the flingy types. From DatEdit x1.5
   */
  namespace FlingyID
  {
  enum Enum
    {
      Scourge                     = 0x00,
      Broodling                   = 0x01,
      Infested_Terran             = 0x02,
      Guardian_Cocoon             = 0x03,
      Defiler                     = 0x04,
      Drone                       = 0x05,
      Egg                         = 0x06,
      Guardian                    = 0x07,
      Hydralisk                   = 0x08,
      Infested_Kerrigan           = 0x09,
      Larva                       = 0x0A,
      Mutalisk                    = 0x0B,
      Overlord                    = 0x0C,
      Queen                       = 0x0D,
      Ultralisk                   = 0x0E,
      Zergling                    = 0x0F,
      Cerebrate                   = 0x10,
      Infested_Command_Center     = 0x11,
      Spawning_Pool               = 0x12,
      Mature_Chrysalis            = 0x13,
      Evolution_Chamber           = 0x14,
      Creep_Colony                = 0x15,
      Hatchery                    = 0x16,
      Hive                        = 0x17,
      Lair                        = 0x18,
      Sunken_Colony               = 0x19,
      Greater_Spire               = 0x1A,
      Defiler_Mound               = 0x1B,
      Queens_Nest                 = 0x1C,
      Nydus_Canal                 = 0x1D,
      Overmind_With_Shell         = 0x1E,
      Overmind_Without_Shell      = 0x1F,
      Ultralisk_Cavern            = 0x20,
      Extractor                   = 0x21,
      Hydralisk_Den               = 0x22,
      Spire                       = 0x23,
      Spore_Colony                = 0x24,
      Arbiter                     = 0x25,
      Archon_Energy               = 0x26,
      Carrier                     = 0x27,
      Dragoon                     = 0x28,
      Interceptor                 = 0x29,
      Probe                       = 0x2A,
      Scout                       = 0x2B,
      Shuttle                     = 0x2C,
      High_Templar                = 0x2D,
      Dark_Templar_Hero           = 0x2E,
      Reaver                      = 0x2F,
      Scarab                      = 0x30,
      Zealot                      = 0x31,
      Observer                    = 0x32,
      Templar_Archives            = 0x33,
      Assimilator                 = 0x34,
      Observatory                 = 0x35,
      Citadel_of_Adun             = 0x36,
      Forge                       = 0x37,
      Gateway                     = 0x38,
      Cybernetics_Core            = 0x39,
      Khaydarin_Crystal_Formation = 0x3A,
      Nexus                       = 0x3B,
      Photon_Cannon               = 0x3C,
      Arbiter_Tribunal            = 0x3D,
      Pylon                       = 0x3E,
      Robotics_Facility           = 0x3F,
      Shield_Battery              = 0x40,
      Stargate                    = 0x41,
      Stasis_Cell_Prison          = 0x42,
      Robotics_Support_Bay        = 0x43,
      Protoss_Temple              = 0x44,
      Fleet_Beacon                = 0x45,
      Battlecruiser               = 0x46,
      Civilian                    = 0x47,
      Dropship                    = 0x48,
      Firebat                     = 0x49,
      Ghost                       = 0x4A,
      Goliath_Base                = 0x4B,
      Goliath_Turret              = 0x4C,
      Sarah_Kerrigan              = 0x4D,
      Marine                      = 0x4E,
      Scanner_Sweep               = 0x4F,
      Wraith                      = 0x50,
      SCV                         = 0x51,
      Siege_Tank_Tank_Base        = 0x52,
      Siege_Tank_Tank_Turret      = 0x53,
      Siege_Tank_Siege_Base       = 0x54,
      Siege_Tank_Siege_Turret     = 0x55,
      Science_Vessel_Base         = 0x56,
      Science_Vessel_Turret       = 0x57,
      Vulture                     = 0x58,
      Spider_Mine                 = 0x59,
      Terran_Academy              = 0x5A,
      Barracks                    = 0x5B,
      Armory                      = 0x5C,
      Comsat_Station              = 0x5D,
      Command_Center              = 0x5E,
      Supply_Depot                = 0x5F,
      Control_Tower               = 0x60,
      Factory                     = 0x61,
      Covert_Ops                  = 0x62,
      Ion_Cannon                  = 0x63,
      Machine_Shop                = 0x64,
      Missile_Turret_Base         = 0x65,
      Crashed_Battlecruiser       = 0x66,
      Physics_Lab                 = 0x67,
      Bunker                      = 0x68,
      Refinery                    = 0x69,
      Immobile_Barracks           = 0x6A,
      Science_Facility            = 0x6B,
      Nuke_Silo                   = 0x6C,
      Nuclear_Missile             = 0x6D,
      Starport                    = 0x6E,
      Engineering_Bay             = 0x6F,
      Terran_Construction_Large   = 0x70,
      Terran_Construction_Small   = 0x71,
      Ragnasaur_Ashworld          = 0x72,
      Rhynadon_Badlands           = 0x73,
      Bengalaas_Jungle            = 0x74,
      Vespene_Geyser              = 0x75,
      Mineral_Field_Type1         = 0x76,
      Mineral_Field_Type2         = 0x77,
      Mineral_Field_Type3         = 0x78,
      Independent_Starport_Unused = 0x79,
      Zerg_Beacon                 = 0x7A,
      Terran_Beacon               = 0x7B,
      Protoss_Beacon              = 0x7C,
      Dark_Swarm                  = 0x7D,
      Flag                        = 0x7E,
      Young_Chrysalis             = 0x7F,
      Psi_Emitter                 = 0x80,
      Data_Disc                   = 0x81,
      Khaydarin_Crystal           = 0x82,
      Mineral_Chunk_Type1         = 0x83,
      Mineral_Chunk_Type2         = 0x84,
      Protoss_Gas_Orb_Type1       = 0x85,
      Protoss_Gas_Orb_Type2       = 0x86,
      Zerg_Gas_Sac_Type1          = 0x87,
      Zerg_Gas_Sac_Type2          = 0x88,
      Terran_Gas_Tank_Type1       = 0x89,
      Terran_Gas_Tank_Type2       = 0x8A,
      Map_Revealer                = 0x8B,
      Start_Location              = 0x8C,
      Fusion_Cutter_Hit           = 0x8D,
      Gauss_Rifle_Hit             = 0x8E,
      C_10_Canister_Rifle_Hit     = 0x8F,
      Gemini_Missiles             = 0x90,
      Fragmentation_Grenade       = 0x91,
      Hellfire_Missile            = 0x92,
      Unknown147                  = 0x93,
      ATS/ATA_Laser_Battery       = 0x94,
      Burst_Lasers                = 0x95,
      Arclite_Shock_Cannon_Hit    = 0x96,
      EMP_Missile                 = 0x97,
      Dual_Photon_Blasters_Hit    = 0x98,
      Particle_Beam_Hit           = 0x99,
      Anti_Matter_Missile         = 0x9A,
      Pulse_Cannon                = 0x9B,
      Psionic_Shockwave_Hit       = 0x9C,
      Psionic_Storm               = 0x9D,
      Yamato_Gun                  = 0x9E,
      Phase_Disruptor             = 0x9F,
      STA/STS_Cannon_Overlay      = 0xA0,
      Sunken_Colony_Tentacle      = 0xA1,
      Venom_Unused_Zerg_Weapon    = 0xA2,
      Acid_Spore                  = 0xA3,
      Unknown164                  = 0xA4,
      Glave_Wurm                  = 0xA5,
      Seeker_Spores               = 0xA6,
      Queen_Spell_Carrier         = 0xA7,
      Plague_Cloud                = 0xA8,
      Consume                     = 0xA9,
      Ensnare                     = 0xAA,
      Needle_Spine_Hit            = 0xAB,
      White_Circle_Invisible      = 0xAC,
      Left_Upper_Level_Door       = 0xAD,
      Right_Upper_Level_Door      = 0xAE,
      Substructure_Left_Door      = 0xAF,
      Substructure_Right_Door     = 0xB0,
      Substructure_Opening_Hole   = 0xB1,
      Floor_Gun_Trap              = 0xB2,
      Floor_Missile_Trap          = 0xB3,
      Wall_Missile_Trap           = 0xB4,
      Wall_Missile_Trap2          = 0xB5,
      Wall_Flame_Trap             = 0xB6,
      Wall_Flame_Trap2            = 0xB7,
      Lurker_Egg                  = 0xB8,
      Devourer                    = 0xB9,
      Lurker                      = 0xBA,
      Dark_Archon_Energy          = 0xBB,
      Dark_Templar_Unit           = 0xBC,
      Medic                       = 0xBD,
      Valkyrie                    = 0xBE,
      Corsair                     = 0xBF,
      Disruption_Web              = 0xC0,
      Overmind_Cocoon             = 0xC1,
      Psi_Disrupter               = 0xC2,
      Warp_Gate                   = 0xC3,
      Power_Generator             = 0xC4,
      XelNaga_Temple              = 0xC5,
      Scantid_Desert              = 0xC6,
      Kakaru_Twilight             = 0xC7,
      Ursadon_Ice                 = 0xC8,
      Optical_Flare_Grenade       = 0xC9,
      Halo_Rockets                = 0xCA,
      Subterranean_Spines         = 0xCB,
      Corrosive_Acid_Shot         = 0xCC,
      Corrosive_Acid_Hit          = 0xCD,
      Neutron_Flare               = 0xCE,
      Uraj                        = 0xCF,
      Khalis                      = 0xD0,
      None                        = 0xD1
    };
  };
};
