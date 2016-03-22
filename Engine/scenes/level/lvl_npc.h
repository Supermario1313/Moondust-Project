#ifndef LVL_NPC_H
#define LVL_NPC_H

#include "lvl_base_object.h"
#include <data_configs/obj_npc.h>
#include <PGE_File_Formats/file_formats.h>
#include <common_features/npc_animator.h>
#include <common_features/event_queue.h>
#include <common_features/pointf.h>
#include "npc_detectors/lvl_base_detector.h"
#include "npc_detectors/lvl_dtc_player_pos.h"
#include "npc_detectors/lvl_dtc_contact.h"
#include "npc_detectors/lvl_dtc_inarea.h"

#include <luabind/luabind.hpp>
#include <lua_inclues/lua.hpp>

#include <QHash>

class LVL_Player;
class LVL_Npc : public PGE_Phys_Object
{
public:
    LVL_Npc(LevelScene *_parent=NULL);
    virtual ~LVL_Npc();
    void init();

    void setScenePointer(LevelScene* _pointer);

    LevelNPC getData();

    int _npc_id;//Current NPC-ID
    LevelNPC data; //Local settings
    PGE_PointF offset;
    PGE_Size frameSize;
    AdvNpcAnimator animator;

    void setDirection(int dir);
    int  direction();
    int _direction;
    float motionSpeed;
    bool  is_scenery;
    bool  is_activity;
    bool  is_shared_animation;
    bool  keep_position_on_despawn;

    bool animated;
    long animator_ID;

    obj_npc *setup;//Global config
    bool isKilled();
    bool killed;
    enum DamageReason{
        DAMAGE_NOREASON=0,
        DAMAGE_STOMPED,   //on stomping to head
        DAMAGE_BY_KICK,   //caused by contact with throwned NPC's
        DAMAGE_BY_PLAYER_ATTACK, //Caused by attaking by player
        //(for example, by sword, by fists, by teeth sting, by blow claws, by whip, etc.)
        DAMAGE_TAKEN, //is Power up taken
        DAMAGE_LAVABURN, //Does NPC burn in lava
        DAMAGE_PITFALL, //Does NPC fell into the pit
        DAMAGE_CUSTOM_REASON
    };
    void doHarm(int damageReason);
    void harm(int damage=1, int damageReason = DAMAGE_NOREASON);

    void talkWith();

    void kill(int damageReason);//! In-Game destroying of NPC with triggering of specific events
    void unregister();          //! Sielent destroying of NPC without triggering of the events

    int taskToTransform;
    int taskToTransform_t;
    void transformTo(long id, int type=0);
    void transformTo_x(long id);

    void update(float tickTime);
    void render(double camX, double camY);
    void setDefaults();
    void Activate();
    void deActivate();

    void updateCollisions();
    void detectCollisions(PGE_Phys_Object *collided);
    bool forceCollideCenter;//!< collide with invizible blocks at center
    float _heightDelta; //Delta of changing height. Need to protect going through block on character switching
    bool onCliff();
    bool cliffDetected;

    /*****************NPC's and blocks******************/
    bool onGround();
    bool  _onGround;
    QHash<int, int > foot_contacts_map;   //!< staying on ground surfaces
    QHash<int, int > foot_sl_contacts_map;//!< Slipery surfaces

    QHash<int, PGE_Phys_Object*> contacted_blocks;
    QHash<int, PGE_Phys_Object*> contacted_bgos;
    QHash<int, PGE_Phys_Object*> contacted_npc;
    QHash<int, PGE_Phys_Object*> contacted_players;

    typedef QHash<int, PGE_Phys_Object*> PlayerColliders;
    QHash<int, PGE_Phys_Object*> collided_top;
    QHash<int, PGE_Phys_Object*> collided_left;
    QHash<int, PGE_Phys_Object*> collided_right;
    QHash<int, PGE_Phys_Object*> collided_bottom;
    QHash<int, PGE_Phys_Object*> collided_center;
    bool  disableBlockCollision;
    bool  disableNpcCollision;
    bool  enablePlayerCollision;
    bool _stucked;

    QVector<PGE_Phys_Object*>   collision_speed_add;
    void updateSpeedAddingStack();
    void applyCorrectionToSA_stack(double offsetX, double offsetY);

    bool    bumpDown;
    bool    bumpUp;
    /***************************************************/
    /*******************Environmept*********************/
    //QHash<int, obj_player_physics > physics;
    QHash<int, int > environments_map;
    //obj_player_physics physics_cur;
    int     environment;
    int     last_environment;
    /*******************Environmept*********************/

    bool reSpawnable;
    bool isActivated;
    bool deActivatable;
    bool wasDeactivated;
    bool offSectionDeactivate;
    int  activationTimeout;

    /********************Detectors**********************/
    QList<BasicDetector >           detectors_dummy; //!< dummy detectors made directly from a base class, for a some tests
    PlayerPosDetector               detector_player_pos; //! Player position detectors (should have alone copy!)
    PlayerPosDetector * lua_installPlayerPosDetector();//! Detects position and direction of nearest player
    QList<InAreaDetector >    detectors_inarea; //! Is player touches selected relative area;
    InAreaDetector * lua_installInAreaDetector(float left, float top, float right, float bottom, luabind::adl::object filters);//! Detects is player(s) are enters into specific area relative to NPC's center
    QList<ContactDetector >        detectors_contact;       //!< Entire list of all detectors
    ContactDetector * lua_installContactDetector();//! Detects contacted elements

    QVector<BasicDetector* >        detectors;       //!< Entire list of all detectors

    /***************************************************/

    /*****Warp*Sprite*****/
    enum WarpingSide{
        WARP_TOP=1,
        WARP_LEFT=2,
        WARP_BOTTOM=3,
        WARP_RIGHT=4,
    };
    ///
    /// \brief setSpriteWarp Changes warping state of a sprite
    /// \param depth
    /// \param direction
    ///
    void setSpriteWarp(float depth, WarpingSide _direction=WARP_BOTTOM, bool resizedBody=false);
    void resetSpriteWarp();
    bool    isWarping;
    int     warpDirectO;
    bool    warpResizedBody;
    float   warpSpriteOffset;
    float   warpFrameW;
    float   warpFrameH;
    /*********************/

    /***************************************************/
    void setWarpSpawn(WarpingSide side=WARP_TOP);
    bool warpSpawing;
    EventQueue<LVL_Npc> event_queue;
    /***************************************************/

    /***********************Generator*******************/
    bool  isGenerator;
    float generatorTimeLeft;
    int   generatorType;
    int   generatorDirection;
    void  updateGenerator(float tickTime);
    /***************************************************/

    /*******************Throwned*by*********************/
    void        resetThrowned();
    void        setThrownedByNpc(long npcID,    LVL_Npc *npcObj);
    void        setThrownedByPlayer(long playerID, LVL_Player *npcObj);
    long        thrownedByNpc();
    LVL_Npc    *thrownedByNpcObj();
    long        thrownedByPlayer();
    LVL_Player *thrownedByPlayerObj();
    long        throwned_by_npc;
    LVL_Npc    *throwned_by_npc_obj;
    long        throwned_by_player;
    LVL_Player *throwned_by_player_obj;
    /***************************************************/

    /*******************Buddies********************/
    //Allows communication between neighour NPC's of same type.
    void             buildBuddieGroup();
    void             updateBuddies(float tickTime);
    void             buildLeaf(QList<LVL_Npc*> &needtochec, QList<LVL_Npc*> *&list, LVL_Npc *leader);
    QList<LVL_Npc*> *buddies_list;//Destroys when was killed last NPC in this group
    bool             buddies_updated;
    LVL_Npc*         buddies_leader;
    /**********************************************/

    class KillEvent
    {
        public:
            KillEvent();
            KillEvent(const KillEvent &ke);
            bool cancel;
            int  reason_code;
            enum killedBy {
                self=0,
                player,
                otherNPC
            };
            int         killed_by;
            LVL_Player* killer_p;
            LVL_Npc*    killer_n;
    };

    class HarmEvent
    {
        public:
            HarmEvent();
            HarmEvent(const HarmEvent &he);
            bool cancel;
            int  damage;
            int  reason_code;
            enum killedBy {
                self=0,
                player,
                otherNPC
            };
            int         killed_by;
            LVL_Player* killer_p;
            LVL_Npc*    killer_n;
    };
    //Additional lua enums

    /********************Lua Stuff*******************
                        .-""""-
                       F   .-'
                      F   J
                     I    I
                      L   `.
                       L    `-._,
                        `-.__.-'
     ***********************************************/
    //Additional lua events
    virtual void lua_onActivated() {}
    virtual void lua_onLoop(float) {}
    virtual void lua_onInit() {}
    virtual void lua_onKill(KillEvent*) {}
    virtual void lua_onHarm(HarmEvent*) {}
    virtual void lua_onTransform(long) {}

    //Additional lua functions
    void lua_setSequenceLeft(luabind::object frames);
    void lua_setSequenceRight(luabind::object frames);
    void lua_setSequence(luabind::object frames);
    void lua_setOnceAnimation(bool en);
    bool lua_animationIsFinished();
    int  lua_frameDelay();
    void lua_setFrameDelay(int ms);
    int lua_activate_neighbours();
    LVL_Npc *lua_spawnNPC(int npcID, int sp_type, int sp_dir, bool reSpawnable=false);

    inline bool not_movable() { return data.nomove; }
    inline void setNot_movable(bool n) { data.nomove=n; }
    inline long contents() { return data.contents; }
    inline void setContents(long c) { data.contents=c; }
    inline long special1() { return data.special_data; }
    inline void setSpecial1(long s) { data.special_data=s; }
    inline long special2() { return data.special_data2; }
    inline void setSpecial2(long s) { data.special_data2=s; }
    inline bool isBoss() { return data.is_boss; }
    inline int  getID() { return _npc_id; }
    inline long getHealth() { return health; }
    inline void setHealth(int _health) { health=_health; }
    inline bool getCollideWithBlocks() { return !disableBlockCollision; }
    inline void setCollideWithBlocks(bool blkcol) { disableBlockCollision=!blkcol; }
    bool isLuaNPC;
    /********************Lua Stuff******************/

    int health;

    /**Layers***/
    void show();
    void hide();

    bool isInited();
private:
    bool _isInited;

};

#endif // LVL_NPC_H
