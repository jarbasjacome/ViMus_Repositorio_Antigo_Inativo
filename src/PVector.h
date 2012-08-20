#ifndef _PVECTOR_H_
#define _PVECTOR_H_

class PVector {
public:
    float x;
    float y;
    PVector(float posX, float posY) {
        x=posX;
        y=posY;
    }
    ~PVector(){}
private:
};

#endif //_PVECTOR_H_
