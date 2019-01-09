#include "TrackPad.h"



void TrackPad::drawDefaultText(Color color) {
    Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 0);
    result_prompt.setString("Left Mouse Button    |  Right Mouse Button  |  ESC\n"
                            "> Track Mouse        | > Confirm Signature  | > Discard Signature");
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::drawTrackCount(Color color) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(500 - 2 * result_prompt.getCharacterSize(), 500 - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(to_string(trackList.size()));
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::drawInfoText(string info, Color color) {
    cout << info << endl;
    window.clear(sf::Color::Black);
    drawDefaultText();

    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 500 - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(info);
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::trackMouseMovement() {
    if (!isTracking) {
        isTracking = true;
        drawInfoText("Tracking ..");
        drawTrackCount();
        window.display();
    }
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));

    track.push_back(mousePos);

    CircleShape shape(2.f);
    shape.setPosition(mousePos);
    window.draw(shape);
    window.display();
}

void TrackPad::storeTrack() {
    if (track.size() < 500) {
        drawInfoText("Too few pixels captured. Please draw bigger or slower.", COLOR_WARN);
        drawTrackCount();

    } else {
        trackList.push_back(track);

        drawInfoText("Track Stored.");
        drawTrackCount();
    }
    track.clear();
    isTracking = false;
    window.display();
}

TrackPad::TrackPad(RenderWindow &window) : window(window){
    window.clear(Color::Black);
    if (!font.loadFromFile("./UbuntuMono-B.ttf"))
    {
        std::cout << "Error loading font\n" ;
    }
    drawDefaultText();
    window.display();
}

vector<Track> TrackPad::getStoredTracks() {
    return trackList;
}

void TrackPad::discardTrack() {
    if (!track.empty()) {
        track.clear();
        drawInfoText("Track Discarded.", COLOR_WARN);
        drawTrackCount();
    } else if(!trackList.empty()) {
        trackList.pop_back();
        drawInfoText("Previous Track Discarded!", COLOR_WARN);
        drawTrackCount();
    }
    isTracking = false;
    window.display();
}

bool TrackPad::isTrackingMouse() {
    return isTracking;
}
