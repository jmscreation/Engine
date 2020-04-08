#include "engine.h"

namespace Engine {

    ArbList<Identifiable*> Identifiable::idmap;
    int Identifiable::currentID = 1;

    Identifiable::Identifiable(int setId): id(setId) {
        if(id == 0) {
            while(currentID == 0 || idmap.has(currentID))
                if(++currentID < 1) currentID = 1;
            id = currentID++;
        }

        idmap.set(id,this);
    }

    Identifiable::~Identifiable() {
        if(idmap.get(id) == this)
            idmap.remove(id);
    }

    Identifiable* Identifiable::getByID(int id) {
        return idmap.get(id);
    }

}