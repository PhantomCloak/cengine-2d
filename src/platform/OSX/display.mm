#include <Cocoa/Cocoa.h>

extern "C" {
    double getScreenScaleFactor() {
        NSScreen *primaryScreen = [NSScreen mainScreen];
        CGFloat scaleFactor = [primaryScreen backingScaleFactor];
        return static_cast<double>(scaleFactor);
    }
}
