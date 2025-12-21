#pragma once

class KeyProcessor {
public:
    enum class Mode {
        NORMAL,
        INVERT,
        BLUR,
        CANNY,
        ZOOMARROWS,
        DRAWLINE,
        PICTS,
        FACE
    };

    void processKey(int key);
    Mode getMode() const;
    double getZoom() const { return zoom; }

private:
    Mode currentMode = Mode::NORMAL;
    double zoom = 1.0;
};
