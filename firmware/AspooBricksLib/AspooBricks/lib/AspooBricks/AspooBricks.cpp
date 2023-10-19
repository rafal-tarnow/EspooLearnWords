#include "AspooBricks.h"

PseudoDNS AspooBrick::pseudoDNS;

void AspooBrick::setId(std::string id)
{
    mId = id;
}

void AspooBrick::begin()
{
    if(!pseudoDNS.isQueriesRunning()){
        pseudoDNS.startQueriesForAllHosts();
    }
}

void AspooBrick::update()
{
   pseudoDNS.update();
}
