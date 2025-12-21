#include "KeyProcessor.hpp"
#include <algorithm>
#include <iostream>
#include "Logger.hpp"

void KeyProcessor::processKey(int key) {
  // if(key!=-1){ std::cout << "Key pressed: " << key << " Mode: " << (int)currentMode << std::endl;}
    if(key!=-1){
        std::stringstream ss;
        ss << "Key pressed: " << key << " Mode changed to: " << (int)currentMode;
        Logger::getInstance().debug(ss.str());}
    switch (key) {
        case '1': currentMode = Mode::NORMAL; break;
        case '2': currentMode = Mode::INVERT; break;
        case '3': currentMode = Mode::BLUR; break;
        case '4': currentMode = Mode::CANNY; break;
        case '5': currentMode = Mode::ZOOMARROWS; break;
        case '6': currentMode = Mode::DRAWLINE; break;
        case '7': currentMode = Mode::PICTS; break;
        case 'f': case 'F': currentMode = Mode::FACE; break;
        case 82: case 2490368: case 65362: zoom *= 1.1; zoom = std::min(zoom, 5.0); break;
        case 84: case 2621440: case 65364: zoom /= 1.1; zoom = std::max(zoom, 0.1); break;
        default: break;
    }
}

KeyProcessor::Mode KeyProcessor::getMode() const {
    return currentMode;
}
