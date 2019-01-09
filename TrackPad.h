#ifndef CARBONCOPYSIGNATUREMATCHING_TRACKPAD_H
#define CARBONCOPYSIGNATUREMATCHING_TRACKPAD_H

#define COLOR_TEXT Color::Green
#define COLOR_WARN Color::Yellow


#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;
using namespace sf;
typedef vector<Vector2f> Track;

class TrackPad {
public:
    explicit TrackPad(RenderWindow &window);
    void drawDefaultText(Color color = COLOR_TEXT);
    void drawTrackCount(Color color = COLOR_TEXT);
    void drawInfoText(string info, Color color = COLOR_TEXT);
    void trackMouseMovement();
    void storeTrack();

    void discardTrack();
    bool isTrackingMouse();

    vector<Track> getStoredTracks();
private:
    RenderWindow &window;
    Font font;

    Track track;
    vector<Track> trackList;
    bool isTracking = false;
};


#endif //CARBONCOPYSIGNATUREMATCHING_TRACKPAD_H
