#ifndef BRUSH_H
#define BRUSH_H
#include <vector>
#include <string>
#include "Variable.h"
class Context;

class Brush
        : public Variable
{
 public:
    static const int DEFAULT_SIZE;
    static const std::string DEFAULT_COLOR;

    Brush();

    int getSize() const;
    std::string getColor() const;
    bool getEnabled() const;

    virtual void getSomeVal(std::vector<std::string> identifiers, Context *context);
    virtual void setSomeVal(std::vector<std::string> identifiers, Context* context);

private:
    int size;
    std::string color;
    bool enabled;

    std::string getColorR() const;
    std::string getColorG() const;
    std::string getColorB() const;
};

#endif // BRUSH_H
