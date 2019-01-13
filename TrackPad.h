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
private:
    RenderWindow &window;
    Font font;
    CircleShape dot;

    Track track;
    vector<Track> trackList;
    int index;

    bool isTracking = false;
    string defaultText;

public:
    TrackPad(RenderWindow &window);
    TrackPad(RenderWindow &window, string defaultText);

    bool loadTrackListFromFile(string path);
    bool setActiveTrackByNumber(int num);
    bool next();
    bool previous();

    void setDefaultText(string text);
    void drawDefaultText(Color color = COLOR_TEXT);
    void drawTrackCount(Color color = COLOR_TEXT);
    void drawTrackIndex(Color color = COLOR_TEXT);
    void drawInfoText(string info, Color color = COLOR_TEXT);
    void trackMouseMovement();
    void storeTrack();
    void clearTrack();

    void stopTracking();
    void drawCurrentTrack();

    void discardTrack();
    bool isTrackingMouse();

    vector<Track> getStoredTracks();
    Track getCurrentTrack();
};


#endif //CARBONCOPYSIGNATUREMATCHING_TRACKPAD_H
