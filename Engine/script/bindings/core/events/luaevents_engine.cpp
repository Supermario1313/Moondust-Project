#include "luaevents_engine.h"



LuaEvent BindingCore_Events_Engine::createInitEngineEvent(LuaEngine *engine)
{
    LuaEvent event = LuaEvent(engine);
    event.setEventName("initEngine");
    return event;
}

LuaEvent BindingCore_Events_Engine::createLoopEvent(LuaEngine *engine, int ticks)
{
    LuaEvent event = LuaEvent(engine);
    event.setEventName("loop");
    event.addParameter(ticks);
    return event;
}
