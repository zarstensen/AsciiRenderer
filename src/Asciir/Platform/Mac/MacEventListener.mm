#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "MacEventListener-objc.h"

namespace Asciir
{
namespace ELInterface
{
    std::pair<size_t, size_t> cococaGetMousePos()
    {
        NSPoint point;
        point = [NSEvent mouseLocation];

        return {point.x, point.y};
    }
}
}
