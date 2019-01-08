#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>


//compile: g++ -c ccsm-track.cpp
//then: g++ ccsm-track.o -o ccsm-track -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;
using namespace sf;

typedef vector<Vector2f> Track;

cxxopts::ParseResult parseTrackingArguments(int argc, char **argv) {
    cxxopts::Options options("ccsm-track", "Carbon Copy Signature Matching - record a specific signature by tracking the mouse movement");
    try {
        options.add_options()
                ("h,help", "Print help")
                ("n,name", "Signature name", cxxopts::value<string>());

        options.parse_positional("name");

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            cout << options.help({"", "Group", "Data", "Training", "Network", "Replication"}) << endl;
            exit(0);
        }
        if (!result.count("name")) {
            cout << "error parsing options: Argument missing: " << " --name" << endl;
            exit(1);
        }
        return result;

    } catch (const cxxopts::OptionException& e)
    {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }

}

void drawDefaultText(RenderWindow &window, Font &font, Color color = Color::Cyan) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 0);
    result_prompt.setString("Left Mouse Button    |  Right Mouse Button  |  ESC\n"
                            "> Track Mouse        | > Confirm Signature  | > Discard Signature");
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void drawTrackCount(RenderWindow &window, Font &font, vector<Track> &trackList, Color color = Color::Cyan) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(500 - 2 * result_prompt.getCharacterSize(), 500 - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(to_string(trackList.size()));
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void drawInfoText(RenderWindow &window, Font &font, string info, Color color = Color::Cyan) {
    cout << info << endl;
    window.clear(sf::Color::Black);
    drawDefaultText(window, font);

    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 500 - result_prompt.getCharacterSize() - 5);
    result_prompt.setString(info);
    result_prompt.setFillColor(color);
    window.draw(result_prompt);
}

void trackMouseMovement(Track &currentTrack, RenderWindow &window) {
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));

    currentTrack.push_back(mousePos);

    CircleShape shape(2.f);
    shape.setPosition(mousePos);
    window.draw(shape);
    window.display();
}

void storeTrack(vector<Track> &trackList, Track &currentTrack) {
    trackList.push_back(currentTrack);
    currentTrack.clear();
}

bool saveTrackListToFile(vector<Track> &trackList, string &file_path) {
    if (trackList.empty()) {
        cout << "No records are saved: Empty Track List" << endl;
        return false;
    }
    cout << "TODO: " << "Saving " << trackList.size() << " signature tracks to: " << file_path << endl;


}

int main(int argc, char** argv) {
    auto args = parseTrackingArguments(argc, argv);
    string path = "./signatures/";


    /*
     * Window setup
     */
    RenderWindow window(VideoMode(500, 500), "Tracking Signatures!");
    window.clear(Color::Black);
    Font font;
    if (!font.loadFromFile("./UbuntuMono-B.ttf"))
    {
        std::cout << "Error loading font\n" ;
    }
    drawDefaultText(window, font);
    window.display();

    /*
     * With open window
     */

    Track track;
    vector<Track> trackList;
    bool onTrack = false;

    cout << "Launch ccsm-track application" << endl;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
                case Event::Closed:
                    path = "./signatures/";
                    path += args["name"].as<string>();
                    path += ".sgn.tsv";
                    saveTrackListToFile(trackList, path);
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left) {
                        if(!onTrack) {
                            drawInfoText(window, font, "Tracking ..");
                            drawTrackCount(window, font, trackList);
                            window.display();
                            onTrack = true;
                        }

                        Event releaseEvent;
                        bool mouseReleased = false;

                        while (!mouseReleased) {

                            trackMouseMovement(track, window);

                            //Catch MouseRelease
                            while (window.pollEvent(releaseEvent)) {
                                if (releaseEvent.type == Event::MouseButtonReleased) {
                                    mouseReleased = true;
                                }
                            }
                        }
                    } else if (event.mouseButton.button == Mouse::Button::Right && onTrack) {
                        storeTrack(trackList, track);
                        drawInfoText(window, font, "Track Stored.");
                        drawTrackCount(window, font, trackList);
                        window.display();
                        onTrack = false;
                    }

                    break;

                case Event::KeyPressed:
                    if(event.key.code == Keyboard::Escape) {
                        if(onTrack) {
                            track.clear();
                            drawInfoText(window, font, "Track Discarded.", Color::Red);
                            drawTrackCount(window, font, trackList);
                            window.display();
                            onTrack = false;
                        } else if(!trackList.empty()) {
                            trackList.pop_back();
                            drawInfoText(window, font, "Previous Track Discarded!", Color::Red);
                            drawTrackCount(window, font, trackList);
                            window.display();
                        }
                    }
                    break;

                default:
                    break;

            }
        }
    }

}
