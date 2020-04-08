#include "engine.h"
using namespace std;

namespace Engine {

    ArbGrid<BaseCollidable*> BaseCollidable::grid(ARB_GRID_CELLS);
    CellIterator BaseCollidable::defaultCellIter;
    PositionIterator BaseCollidable::defaultPosIter;
    AllIterator BaseCollidable::defaultAllIter;
    RectIterator BaseCollidable::defaultRectIter;

    static int cellOffsets[] = {0,0, -1,-1, 0,-1, 1,-1, -1,0, 1,0, -1,1, 0,1, 1,1};

    BaseCollidable::BaseCollidable(float x,float y): mask(NULL) {
        int cx = int(floor(x / COLLIDABLE_REGION_SIZE)),
            cy = int(floor(y / COLLIDABLE_REGION_SIZE));

        grid.cellAdd(cx,cy,this);
    }

    BaseCollidable::~BaseCollidable() {
        if(mask != NULL)
            delete mask;
    }

    void BaseCollidable::beginCell(float x,float y,CellIterator& iter) {
        int iterX = int(floor(x / COLLIDABLE_REGION_SIZE)),
            iterY = int(floor(y / COLLIDABLE_REGION_SIZE));
        grid.iterateCellBegin(iterX, iterY, iter.arbIter);
    }

    BaseCollidable* BaseCollidable::iterateCell(CellIterator& iter) {
        BaseCollidable* c;
        return grid.iterateCellNext(iter.arbIter);
    }

    void BaseCollidable::beginPosition(float x,float y,PositionIterator& iter) {
        iter.x = int(floor(x / COLLIDABLE_REGION_SIZE));
        iter.y = int(floor(y / COLLIDABLE_REGION_SIZE));
        iter.cell = 0;
        grid.iterateCellBegin(iter.x + cellOffsets[0], iter.y + cellOffsets[1], iter.arbIter);
    }

    BaseCollidable* BaseCollidable::iteratePosition(PositionIterator& iter) {
        if(iter.cell == -1) return NULL;
        BaseCollidable* c;
        do {
            c = grid.iterateCellNext(iter.arbIter);
            if(c == NULL) {
                if(++iter.cell==9) {
                    iter.cell = -1;
                    return NULL;
                }
                grid.iterateCellBegin(iter.x + cellOffsets[iter.cell*2], iter.y + cellOffsets[iter.cell*2+1], iter.arbIter);
            }
        } while(c == NULL);
        return c;
    }

    void BaseCollidable::beginAll(AllIterator& iter) {
        grid.iterateAllBegin(iter.arbIter);
    }

    BaseCollidable* BaseCollidable::iterateAll(AllIterator& iter) {
        return grid.iterateAllNext(iter.arbIter);
    }

    void BaseCollidable::beginRect(float x1,float y1,float x2,float y2,RectIterator& iter) {
        float tmp;
        if(x1 > x2) tmp = x1, x1 = x2, x2 = tmp;
        if(y1 > y2) tmp = y1, y1 = y2, y2 = tmp;

        int _x1 = int(floor(x1 / COLLIDABLE_REGION_SIZE)),
            _y1 = int(floor(y1 / COLLIDABLE_REGION_SIZE)),
            _x2 = int(floor(x2 / COLLIDABLE_REGION_SIZE)),
            _y2 = int(floor(y2 / COLLIDABLE_REGION_SIZE));
        iter.cell = 0;
        iter.x = _x1;
        iter.y = _y1;
        iter.w = _x2 - _x1 + 1;
        iter.h = _y2 - _y1 + 1;
        iter.x1 = x1;
        iter.y1 = y1;
        iter.x2 = x2;
        iter.y2 = y2;
        grid.iterateCellBegin(iter.x, iter.y, iter.arbIter);
    }

    BaseCollidable* BaseCollidable::iterateRect(RectIterator& iter) {
        if(iter.cell == -1) return NULL;
        BaseCollidable* c;
        do {
            c = grid.iterateCellNext(iter.arbIter);
            if(c == NULL) {
                if(++iter.cell >= iter.w * iter.h) {
                    iter.cell = -1;
                    return NULL;
                }
                grid.iterateCellBegin(iter.x + iter.cell % iter.w, iter.y + iter.cell / iter.w, iter.arbIter);
            }
        } while(c == NULL || c->x() < iter.x1 || c->x() >= iter.x2 || c->y() < iter.y1 || c->y() >= iter.y2);
        return c;
    }

    void BaseCollidable::beginCollision(PositionIterator& iter) {
        beginPosition(x(),y(),iter);
    }

    BaseCollidable* BaseCollidable::iterateCollidable(PositionIterator& iter) {
        if(iter.cell == -1) return NULL;
        BaseCollidable* c;
        do {
            c = grid.iterateCellNext(iter.arbIter);
            if(c == NULL) {
                if(++iter.cell==9) {
                    iter.cell = -1;
                    return NULL;
                }
                grid.iterateCellBegin(iter.x + cellOffsets[iter.cell*2], iter.y + cellOffsets[iter.cell*2+1], iter.arbIter);
            }
        } while(c == NULL || c == this);
        return c;
    }

    void BaseCollidable::move(float ox,float oy,float nx,float ny) {
        int ocx = int(floor(ox / COLLIDABLE_REGION_SIZE)),
            ocy = int(floor(oy / COLLIDABLE_REGION_SIZE)),
            ncx = int(floor(nx / COLLIDABLE_REGION_SIZE)),
            ncy = int(floor(ny / COLLIDABLE_REGION_SIZE));
        if(mask != NULL) {
            mask->x = nx;
            mask->y = ny;
        }
        if(ocx != ncx || ocy != ncy) {
            grid.cellRemove(ocx,ocy,this);
            grid.cellAdd(ncx,ncy,this);
        }
    }

    void BaseCollidable::remove(float x,float y) {
        int cx = int(floor(x / COLLIDABLE_REGION_SIZE)),
            cy = int(floor(y / COLLIDABLE_REGION_SIZE));

        grid.cellRemove(cx,cy,this);
    }

    Collidable::Collidable(float x,float y):
        BaseCollidable(x,y) {}

    Collidable::~Collidable() {}

    void Collidable::move(float dx,float dy) {
        BaseCollidable::move(_x,_y,_x+dx,_y+dy);
        _x += dx; _y += dy;
    }

    void Collidable::x(float x) {
        BaseCollidable::move(_x,_y,x,_y);
        _x = x;
    }

    void Collidable::y(float y) {
        BaseCollidable::move(_x,_y,_x,y);
        _y = y;
    }

    void Collidable::setPosition(float x,float y) {
        BaseCollidable::move(_x,_y,x,y);
        _x = x; _y = y;
    }

}
