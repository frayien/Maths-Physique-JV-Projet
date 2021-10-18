#ifndef FRAYIEN_ISHAPE
#define FRAYIEN_ISHAPE

class IShape
{   
public:
    IShape() = default;
    virtual ~IShape() = default;

    virtual void draw() const = 0;
};


#endif // FRAYIEN_ISHAPE
