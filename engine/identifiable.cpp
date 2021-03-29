#include "engine.h"

namespace Engine {

    ArbList<Identifiable*> Identifiable::idmap;
    unsigned int Identifiable::currentID = 1;
    bool Identifiable::IDENTITY_COLLISION_ERRORS = false;

    Identifiable::Identifiable(unsigned int setId): id(setId) {
        if(id == 0) {
            while(currentID == 0 || idmap.has(currentID))
                if(++currentID < 1) currentID = 1;
            id = currentID++;
        } else {
			Identifiable *other = idmap.get(id);
			if(other != nullptr) {
                if(IDENTITY_COLLISION_ERRORS)
                    throw std::runtime_error("Identifiable::Identifiable(): IDENTITY_COLLISION");
				other->id = 0; // remove/replace the id if already exists
			}
		}

        idmap.set(id,this);
    }

    Identifiable::~Identifiable() {
        if(id != 0 && idmap.get(id) == this)
            idmap.remove(id);
    }

    Identifiable* Identifiable::getByID(unsigned int id) {
        return idmap.get(id);
    }

}
