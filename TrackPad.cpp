#include "TrackPad.h"



TrackPad::TrackPad(RenderWindow &window) : window(window){
    dot = CircleShape(2.f, 4);
    dot.setOrigin(Vector2f(2.f,2.f));
    window.clear(Color::Black);
    if (!font.loadFromFile("./UbuntuMono-B.ttf"))
    {
        std::cout << "Error loading font\n" ;
    }
    drawDefaultText();
    window.display();
}

TrackPad::TrackPad(RenderWindow &window, string defaultText) : window(window), defaultText(std::move(defaultText)) {
    dot = CircleShape(2.f, 4);
    dot.setOrigin(Vector2f(2.f,2.f));


    window.clear(Color::Black);
    if (!font.loadFromFile("./UbuntuMono-B.ttf"))
    {
        std::cout << "Error loading font\n" ;
    }
    drawDefaultText();
    window.display();
}

void TrackPad::drawDefaultText(Color color) {
    Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 0);
    result_prompt.setString(defaultText);
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::drawTrackCount(Color color) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(window.getSize().y - 2 * result_prompt.getCharacterSize(), window.getSize().y - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(to_string(trackList.size()));
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::drawTrackIndex(Color color) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(window.getSize().y - 4 * result_prompt.getCharacterSize(), window.getSize().y - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(to_string(index) + "/" + to_string(trackList.size() - 1));
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
    result_prompt.setPosition(0, window.getSize().y - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(info);
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void TrackPad::trackMouseMovement() {

    //Tracking init if not already in track mode
    if (!isTracking) {
        isTracking = true;
        if (track.empty()) {
            drawInfoText("Tracking ..");
            drawTrackCount();
        }
    }
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));


    //Discard mouse position if it has not changed
    if(!track.empty()) {
        Vector2f prevPos = track.back();
        if (prevPos.x == mousePos.x && prevPos.y == mousePos.y) {
            return;
        }
    }

    //Save and display mouse position
    track.push_back(mousePos);

    dot.setPosition(mousePos);
    window.draw(dot);
    window.display();
    }

void TrackPad::storeTrack() {
    if (track.size() < 5) {
        drawInfoText("Too few pixels captured. Please draw bigger or slower.", COLOR_WARN);
        drawTrackCount();

    } else {
        cout << track.size() << endl;
        trackList.push_back(track);

        drawInfoText("Track Stored.");
        drawTrackCount();
    }
    track.clear();
    isTracking = false;
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

void TrackPad::setDefaultText(string text) {
    defaultText = text;
}

void TrackPad::stopTracking() {
    isTracking = false;
}

void TrackPad::drawCurrentTrack() {
    window.clear(sf::Color::Black);
    drawDefaultText();
    drawTrackIndex();


    for (Vector2f pos : track) {
        dot.setPosition(pos);
        window.draw(dot);
    }
    window.display();

}

bool TrackPad::loadTrackListFromFile(string path) {
    trackList.clear();

    ifstream file(path, ios::app);
    if (!file.good()) {
        cout << path << " is corrupt" << endl;
        return false;
    }


    while (!file.eof()) {
        string line;
        getline(file, line);

        stringstream ss(line);
        Track cTrack;
        int x,y;
        while(ss >> x >> y) {
             cTrack.push_back(Vector2f(x,y));
        }
        if(!cTrack.empty()) { //Drop empty lines
            trackList.push_back(cTrack);
        }
    }
    return true;
}

bool TrackPad::setActiveTrackByNumber(int num) {
    if (num >= trackList.size()) {
        return false;
    }
    track = trackList[num];
    index = num;
    return true;
}

bool TrackPad::next() {
    if (index >= trackList.size() - 1) {
        return false;
    }
    ++index;
    track = trackList[index];
    return true;
}

bool TrackPad::previous() {
    if (index == 0) {
        return false;
    }
    --index;
    track = trackList[index];
    return true;
}
