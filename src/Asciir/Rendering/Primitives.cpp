#include "Primitives.h"
#include "Asciir/Maths/Maths.h"

namespace Asciir
{
    Coord Quad::centrePoint(const Transform& transform) const
    {
        // simply get the centre of the quad as if it was not transformed, and then reverse the transformation to the resulting point.
        return transform.applyTransform(offset + (size) / 2);
    }

    bool Quad::isInside(Coord coord, const Transform& transform) const
    {
        // first, reverse the transformation on coord.
        // this makes sure we do not have to transform the quad itself, making the calculations more expensive.

        coord = transform.reverseTransform(coord) - offset;

        return    0 <= coord.x && coord.x <= size.x
               && 0 <= coord.y && coord.y <= size.y;
    }
    
    bool Quad::isInsideGrid(const Coord& coord, Real resolution, const Transform& transform) const
    {
        // simply create a new quad which is fitted to a grid corresponding to the resolution
        Quad grid_quad(Coord(ceil(size.x, resolution), floor(size.y, resolution)), Coord(floor(offset.x, resolution), ceil(offset.y, resolution)));
        return grid_quad.isInside(coord, transform);
    }
}
