#ifndef BRUSH_H
#define BRUSH_H
#include <string>

class Brush
{
 public:
    static const int DEFAULT_SIZE;
    static const std::string DEFAULT_COLOR;

    Brush();

    int getSize() const;
    std::string getColor() const;

private:
    int size;
    std::string color;

    std::string getColorR() const;
    std::string getColorG() const;
    std::string getColorB() const;
};

#endif // BRUSH_H
