#ifndef LVL_NPC_H
#define LVL_NPC_H

#include "lvl_base_object.h"
#include <data_configs/obj_npc.h>
#include <PGE_File_Formats/file_formats.h>
#include <common_features/npc_animator.h>
#include <common_features/event_queue.h>
#include <common_features/pointf.h>
#include "npc_detectors/lvl_base_detector.h"

#include <luabind/luabind.hpp>
#include <lua_inclues/lua.hpp>

#include <QHash>

class LVL_Player;
class LVL_Npc : public PGE_Phys_Object
{
public:
    LVL_Npc();
    virtual ~LVL_Npc();
    void init();

    LevelNPC getData();

    LevelNPC data; //Local settings
    PGE_PointF offset;
    PGE_Size frameSize;
    AdvNpcAnimator animator;

    void setDirection(int dir);
    int  direction();
    int _direction;
    float motionSpeed;
    bool  is_scenery;

    bool animated;
    long animator_ID;

    obj_npc *setup;//Global config
    bool isKilled();
    bool killed;
    void harm(int damage=1);
    void kill();

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
    void solveCollision(PGE_Phys_Object *collided);
    bool forceCollideCenter;//!< collide with invizible blocks at center
    float _heightDelta; //Delta of changing height. Need to protect going through block on character switching

    /*****************NPC's and blocks******************/
    bool onGround();
    bool  _onGround;
    QHash<int, int > foot_contacts_map;   //!< staying on ground surfaces
    QHash<int, int > foot_sl_contacts_map;//!< Slipery surfaces

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
    bool _stucked;

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
    int  activationTimeout;

    /********************Detectors**********************/
    QList<BasicDetector >    detectors_dummy; //!< dummy detectors made directly from a base class, for a some tests
    QVector<BasicDetector* > detectors;       //!< Entire list of all detectors
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

    //Additional lua events
    virtual void lua_onActivated() {}
    virtual void lua_onLoop(float) {}
    virtual void lua_onInit() {}

    //Additional lua functions
    void lua_setSequenceLeft(luabind::object frames);
    void lua_setSequenceRight(luabind::object frames);
    void lua_setSequence(luabind::object frames);
    void lua_setOnceAnimation(bool en);
    bool lua_animationIsFinished();
    int  lua_frameDelay();
    void lua_setFrameDelay(int ms);
    inline long special1() { return data.special_data; }
    inline long special2() { return data.special_data2; }
    inline bool isBoss() { return data.legacyboss; }
    inline int getID() { return data.id; }
    bool isLuaNPC;

    int health;


    bool isInited();
private:
    bool _isInited;

};

#endif // LVL_NPC_H
