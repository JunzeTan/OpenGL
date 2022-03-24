#define OBJECT_H

#include <glm/glm.hpp>

#include "TTexture.h"
#include "objLoader.h"
#include "MyObj.h"

enum ObjectType {NONE,CHARACTER, BUILDING,COIN};

class ResourceManager
{
public:
    static MyObj *car;
    static TTexture *texCar;
    static objLoader *stone;
    static TTexture *texStone;
    static objLoader *coin;
    static TTexture *texCoin;
    static void LoadResource();
};

class Object
{
public:
    glm::vec3 pos;
    ObjectType type;
    Object(float x = 0, float y = 0, float z = 0);
    virtual void Render() = 0;
protected:
    void Render(float r, float g, float b, float s);
};

class TrackObject: public Object
{
protected:
    static float trackWidth;
    int trackIndex;
    float GetX() const
    {
        return (trackIndex - 1) *trackWidth;
    }
public:
    int value;
    TrackObject(int trackIndex_,int value_) :trackIndex(trackIndex_),value(value_) {}
    TrackObject(int trackIndex_,int value_,float y,float z):trackIndex(trackIndex_), value(value_)
    {
        pos.x = GetX();
        pos.y = y;
        pos.z = z;
    }
    static void SetTrackWidth(float trackWidth_);
    int GetTrackIndex() const { return trackIndex; }
};

class Player : public TrackObject
{
private:
public:
    Player();
    virtual void Render();
    void MoveLeft();
    void MoveRight();
};

class Building : public TrackObject
{
private:
    float angle;
public:
    Building(int trackIndex, float y = 0, float z = 0);
    virtual void Render();
};

class Coin : public TrackObject
{
private:
    float angle;
public:
    Coin(int trackIndex, float y = 0, float z = 0);
    virtual void Render();
};